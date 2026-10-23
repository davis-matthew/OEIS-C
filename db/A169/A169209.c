/*
 * A169209  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of words of length n over alphabet {0,1,2,3} with
 *            no adjacent equal letters and no subword of length 28 that
 *            alternates between two distinct letters starting with the
 *            smaller of the two letters. (The forbidden subwords are i j i j
 *            … (28 letters) with i < j.) The sequence starts with...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a small Aho–Corasick automaton for the
   six forbidden patterns and uses dynamic programming over the
   automaton states.  The algorithm runs in O(n) time and
   uses O(states) memory, where states ≤ 12·28+1 = 337.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----------  Automaton structure  ---------- */
#define MAX_STATES 500          /* enough for 12·28+1 = 337 */
#define ALPHA      4            /* alphabet size */

typedef struct {
    int next[ALPHA];
    int fail;
    int out;          /* non‑zero if this state corresponds to a
                         forbidden word (full length reached)   */
} Node;

/* ----------  Static automaton data  ---------- */
static Node nodes[MAX_STATES];
static size_t state_cnt = 0;

/* ----------  DP buffers  ---------- */
static seq_t *dp_prev = NULL;   /* will be allocated after automaton
                                   is built */
static seq_t *dp_next = NULL;
static size_t dp_size  = 0;     /* number of elements in each dp array */

/* ----------  Helper for automaton construction  ---------- */
static void build_automaton(void)
{
    size_t i, j, k, s, t;
    int cur, nxt, f;
    int q[MAX_STATES], qh = 0, qt = 0;

    /* initialise root */
    state_cnt = 1;
    for (i = 0; i < ALPHA; ++i) nodes[0].next[i] = -1;
    nodes[0].fail = 0;
    nodes[0].out  = 0;

    /* insert the six forbidden patterns:
          for each unordered pair (i<j) the word starts with i
          and alternates i j i j … 28 letters (14 pairs)         */
    for (i = 0; i < ALPHA; ++i) {
        for (j = i + 1; j < ALPHA; ++j) {
            cur = 0;
            for (k = 0; k < 28; ++k) {
                int ch = (k % 2 == 0 ? i : j);
                if (nodes[cur].next[ch] == -1) {
                    /* create new node */
                    nodes[state_cnt].next[0] = nodes[state_cnt].next[1] =
                    nodes[state_cnt].next[2] = nodes[state_cnt].next[3] = -1;
                    nodes[state_cnt].fail = 0;
                    nodes[state_cnt].out  = 0;
                    nodes[cur].next[ch] = state_cnt++;
                }
                cur = nodes[cur].next[ch];
            }
            nodes[cur].out = 1;   /* full word reached */
        }
    }

    /* build failure links and complete transitions (Aho–Corasick) */
    for (s = 0; s < ALPHA; ++s) {
        nxt = nodes[0].next[s];
        if (nxt != -1) {
            nodes[nxt].fail = 0;
            q[qt++] = nxt;
        } else {
            nodes[0].next[s] = 0;          /* missing edge goes to root */
        }
    }

    while (qh < qt) {
        int r = q[qh++];
        for (s = 0; s < ALPHA; ++s) {
            nxt = nodes[r].next[s];
            if (nxt != -1) {
                f = nodes[r].fail;
                nodes[nxt].fail = nodes[f].next[s];
                nodes[nxt].out  |= nodes[ nodes[nxt].fail ].out;
                q[qt++] = nxt;
            } else {
                nodes[r].next[s] = nodes[ nodes[r].fail ].next[s];
            }
        }
    }

    /* allocate DP buffers */
    dp_size = state_cnt * ALPHA;
    dp_prev = (seq_t *)malloc(dp_size * sizeof(seq_t));
    dp_next = (seq_t *)malloc(dp_size * sizeof(seq_t));
    memset(dp_prev, 0, dp_size * sizeof(seq_t));
    memset(dp_next, 0, dp_size * sizeof(seq_t));
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    size_t i, j, s, t, idx, pos;
    int prev, nxt;
    seq_t val, total;

    /* initialize automaton and DP buffers on first call */
    if (state_cnt == 0) build_automaton();

    if (n == 0) return (seq_t)1;

    /* initial placement of the first letter */
    memset(dp_next, 0, dp_size * sizeof(seq_t));
    for (prev = 0; prev < ALPHA; ++prev) {
        int st = nodes[0].next[prev];
        if (nodes[st].out) continue;               /* should not happen */
        idx = prev * state_cnt + st;
        dp_next[idx] = (seq_t)1;
    }

    /* DP for the remaining positions */
    for (pos = 1; pos < n; ++pos) {
        /* swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
        memset(dp_next, 0, dp_size * sizeof(seq_t));

        /* iterate over all previous states */
        for (prev = 0; prev < ALPHA; ++prev) {
            for (s = 0; s < state_cnt; ++s) {
                val = dp_prev[prev * state_cnt + s];
                if (val == 0) continue;
                for (nxt = 0; nxt < ALPHA; ++nxt) {
                    if (nxt == prev) continue;            /* no repeats */
                    t = nodes[s].next[nxt];
                    if (nodes[t].out) continue;           /* forbidden */
                    idx = nxt * state_cnt + t;
                    dp_next[idx] += val;
                }
            }
        }
    }

    /* sum all surviving states */
    total = 0;
    for (i = 0; i < dp_size; ++i) {
        total += dp_next[i];
    }
    return total;
}
