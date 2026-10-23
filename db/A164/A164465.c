/*
 * A164465  Number of binary strings of length n with no substrings equal to
 *          0001 0100 or 1010.
 * https://oeis.org/A164465
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of binary strings of length n with no substrings equal to
 *            0001, 0100 or 1010. The offset is 4, so a(4)=13
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a deterministic finite automaton derived from the
   forbidden patterns.  It counts all length‑n binary strings that never
   reach an accepting (pattern) state.  The computation is O(n) with a
   very small constant factor.  */

#include <stdbool.h>
#include <stddef.h>

#define MAX_STATES 20

typedef struct {
    int next[2];
    int fail;
    bool out;
} Node;

// Static automaton, built once
static Node nodes[MAX_STATES];
static int state_cnt = 0;
static bool automaton_ready = false;

// Build the Aho–Corasick automaton for the three forbidden patterns.
static void build_automaton(void)
{
    // Initialize root
    int nodecnt = 1;                     // node 0 reserved
    nodes[0].next[0] = nodes[0].next[1] = -1;
    nodes[0].fail = 0;
    nodes[0].out  = false;

    // Insert patterns
    const char *patterns[3] = { "0001", "0100", "1010" };
    for (int p = 0; p < 3; ++p) {
        int cur = 0;
        const char *s = patterns[p];
        while (*s) {
            int bit = (*s++ - '0');
            int nxt = nodes[cur].next[bit];
            if (nxt == -1) {
                // create new node
                nodes[nodecnt].next[0] = nodes[nodecnt].next[1] = -1;
                nodes[nodecnt].fail = 0;
                nodes[nodecnt].out  = false;
                nxt = nodecnt;
                ++nodecnt;
                nodes[cur].next[bit] = nxt;
            }
            cur = nxt;
        }
        nodes[cur].out = true;   // terminal for this pattern
    }

    // Build failure links
    int queue[MAX_STATES];
    int qh = 0, qt = 0;
    for (int b = 0; b < 2; ++b) {
        int child = nodes[0].next[b];
        if (child != -1) {
            nodes[child].fail = 0;
            queue[qt++] = child;
        } else {
            nodes[0].next[b] = 0;
        }
    }
    while (qh < qt) {
        int r = queue[qh++];
        for (int b = 0; b < 2; ++b) {
            int s = nodes[r].next[b];
            if (s != -1) {
                nodes[s].fail = nodes[nodes[r].fail].next[b];
                nodes[s].out  = nodes[s].out || nodes[nodes[s].fail].out;
                queue[qt++] = s;
            } else {
                nodes[r].next[b] = nodes[nodes[r].fail].next[b];
            }
        }
    }

    state_cnt = nodecnt;
    automaton_ready = true;
}

// Counts valid binary strings of length n.
seq_t a(idx_t n)
{
    // Ensure automaton is built once.
    if (!automaton_ready)
        build_automaton();

    // DP arrays for current & next step.
    seq_t dp[20], next[20];
    for (int i = 0; i < state_cnt; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        // zero next counts
        for (int i = 0; i < state_cnt; ++i)
            next[i] = 0;

        for (int i = 0; i < state_cnt; ++i) {
            seq_t cur = dp[i];
            if (!cur) continue;
            for (int b = 0; b < 2; ++b) {
                int nxt_state = nodes[i].next[b];
                if (nodes[nxt_state].out) continue;
                next[nxt_state] += cur;
            }
        }

        // swap dp and next
        for (int i = 0; i < state_cnt; ++i)
            dp[i] = next[i];
    }

    // Sum all non‑terminal states
    seq_t result = 0;
    for (int i = 0; i < state_cnt; ++i)
        result += dp[i];

    return result;
}
