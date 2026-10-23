/*
 * A164489  Number of binary strings of length n with no substrings equal to
 *          0001 1011 or 1100.
 * https://oeis.org/A164489
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n containing no
 *            substrings 0001, 1011, or 1100
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 52, u64 n <= 106,
 *            u128 n <= 215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int next[2];
    int fail;
    bool out;
} Node;

static Node nodes[64];
static int nodeCnt;
static bool built = false;

/* Build the Aho–Corasick automaton for the forbidden patterns. */
static void buildAutomaton(void) {
    const char *patterns[3] = { "0001", "1011", "1100" };
    const int PAT_COUNT = 3;
    const int MAX_STATES = 64;

    /* root node */
    nodeCnt = 1;
    nodes[0].next[0] = nodes[0].next[1] = -1;
    nodes[0].fail = 0;
    nodes[0].out = false;

    /* insert patterns */
    for (int p = 0; p < PAT_COUNT; ++p) {
        const char *s = patterns[p];
        int u = 0;
        for (; *s; ++s) {
            int bit = (*s == '0') ? 0 : 1;
            if (nodes[u].next[bit] == -1) {
                nodes[nodeCnt].next[0] = nodes[nodeCnt].next[1] = -1;
                nodes[nodeCnt].fail = 0;
                nodes[nodeCnt].out = false;
                nodes[u].next[bit] = nodeCnt++;
            }
            u = nodes[u].next[bit];
        }
        nodes[u].out = true;          /* terminal node for a forbidden word */
    }

    /* build failure links */
    int queue[MAX_STATES];
    int qh = 0, qt = 0;
    for (int b = 0; b < 2; ++b) {
        int v = nodes[0].next[b];
        if (v != -1) {
            nodes[v].fail = 0;
            queue[qt++] = v;
        } else {
            nodes[0].next[b] = 0;
        }
    }
    while (qh < qt) {
        int r = queue[qh++];
        for (int b = 0; b < 2; ++b) {
            int v = nodes[r].next[b];
            if (v != -1) {
                int f = nodes[nodes[r].fail].next[b];
                nodes[v].fail = f;
                nodes[v].out |= nodes[f].out;
                queue[qt++] = v;
            } else {
                nodes[r].next[b] = nodes[nodes[r].fail].next[b];
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!built) {
        buildAutomaton();
        built = true;
    }

    /* DP arrays */
    seq_t *dp = (seq_t *)calloc(nodeCnt, sizeof(seq_t));
    seq_t *ndp = (seq_t *)calloc(nodeCnt, sizeof(seq_t));
    if (!dp || !ndp) {
        free(dp);
        free(ndp);
        return 0;                       /* allocation failure handler */
    }
    dp[0] = 1;                          /* start at the root */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < nodeCnt; ++i) ndp[i] = 0;
        for (int s = 0; s < nodeCnt; ++s) {
            if (nodes[s].out) continue;
            seq_t val = dp[s];
            if (!val) continue;
            for (int b = 0; b < 2; ++b) {
                int t = nodes[s].next[b];
                if (!nodes[t].out) ndp[t] += val;
            }
        }
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* sum all non‑terminal states */
    seq_t ans = 0;
    for (int s = 0; s < nodeCnt; ++s) {
        if (!nodes[s].out) ans += dp[s];
    }

    free(dp);
    free(ndp);
    return ans;
}
