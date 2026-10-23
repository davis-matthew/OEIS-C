/*
 * A169257  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in a Coxeter group
 *            with 4 generators S_i and relations S_i^2 = I, (S_i S_j)^29 = I
 *            (i ≠ j). The set of reduced words is regular; it consists of all
 *            words over {0,1,2,3} with no equal consecutive letters and with
 *            no alternating subword of length 29 betwee...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* ---------- Automaton data structures ---------- */
#define MAX_STATES 350            /* root + 12 patterns of length 29 */

typedef struct {
    int next[4];                  /* transitions for symbols 0..3 */
    int fail;                     /* failure link */
    bool out;                     /* terminal (forbidden) state */
    int last;                     /* last symbol that led to this node (-1 for root) */
} Node;

/* static automaton state */
static Node nodes[MAX_STATES];
static int node_cnt = 0;      /* number of states created */
static bool automaton_built = false;

/* ---------- Helper: add a pattern to the trie ---------- */
static void add_pattern(const int *pat, int len) {
    int v = 0;
    for (int i = 0; i < len; ++i) {
        int c = pat[i];
        if (nodes[v].next[c] == -1) {
            int u = node_cnt++;
            nodes[v].next[c] = u;
            nodes[u].fail = 0;
            nodes[u].out  = false;
            nodes[u].last = c;
            for (int cc = 0; cc < 4; ++cc) nodes[u].next[cc] = -1;
        }
        v = nodes[v].next[c];
    }
    nodes[v].out = true;            /* pattern end */
}

/* ---------- Build the Aho-Corasick automaton ---------- */
static void build_automaton(void)
{
    if (automaton_built) return;
    automaton_built = true;

    /* initialise root */
    nodes[0].fail = 0;
    nodes[0].out  = false;
    nodes[0].last = -1;
    for (int c = 0; c < 4; ++c) nodes[0].next[c] = -1;
    node_cnt = 1;

    /* generate all 12 alternating patterns of length 29 */
    const int L = 29;
    int pat[L];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (i == j) continue;
            for (int t = 0; t < L; ++t) pat[t] = (t % 2 == 0) ? i : j;
            add_pattern(pat, L);
        }

    /* build failure links and complete transitions */
    int queue[MAX_STATES];
    int qh = 0, qt = 0;
    for (int c = 0; c < 4; ++c) {
        int u = nodes[0].next[c];
        if (u != -1) {
            nodes[u].fail = 0;
            queue[qt++] = u;
        } else {
            nodes[0].next[c] = 0;
        }
    }

    while (qh < qt) {
        int v = queue[qh++];
        nodes[v].out = nodes[v].out || nodes[nodes[v].fail].out;
        for (int c = 0; c < 4; ++c) {
            int u = nodes[v].next[c];
            if (u != -1) {
                nodes[u].fail = nodes[nodes[v].fail].next[c];
                queue[qt++] = u;
            } else {
                nodes[v].next[c] = nodes[nodes[v].fail].next[c];
            }
        }
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    build_automaton();
    const int S = node_cnt;                /* number of states */

    /* DP arrays for current and next positions */
    static seq_t cur[MAX_STATES];
    static seq_t nxt[MAX_STATES];

    /* initialise */
    for (int i = 0; i < S; ++i) cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;                     /* empty word at root */

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int i = 0; i < S; ++i) nxt[i] = (seq_t)0;
        for (int s = 0; s < S; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            int last = nodes[s].last;      /* -1 for root, else 0..3 */
            for (int c = 0; c < 4; ++c) {
                if (c == last) continue;  /* no consecutive repeats */
                int t = nodes[s].next[c];
                if (!nodes[t].out) nxt[t] += val;
            }
        }
        for (int i = 0; i < S; ++i) cur[i] = nxt[i];
    }

    seq_t ans = (seq_t)0;
    for (int s = 0; s < S; ++s)
        if (!nodes[s].out) ans += cur[s];

    return ans;
}
