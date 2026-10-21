/*
 * A200662  Number of 0..2 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its three previous neighbors modulo 3.
 * https://oeis.org/A200662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=3, a(2)=6; for n≥3,
 *     a(n) = ∑_{s valid triple} (A^(n‑3))[s][*]
 * where A is the 27×27 adjacency matrix
 *     (a,b,c) → (b,c,d) iff d ≥ (a+b+c) mod 3.
 * The implementation uses a simple DP on the 27 states.
 * No external dependencies beyond <stdbool.h>.
 */

#include <stdbool.h>

#define NSTATES 27
#define NIN 3
#define NOUT 3
#define MAXNEXT 3     /* each state has at most 3 outgoing edges */

/* initialise the valid‑triples vector (V) and adjacency lists (next_idx, next_cnt) */
static bool init_done = false;
static seq_t V[NSTATES];               /* V[s] = 1 if triple s satisfies the first two constraints */
static int  next_idx[NSTATES][MAXNEXT];
static int  next_cnt[NSTATES];

/* convert a triple to state index */
static inline int state_index(int a, int b, int c)
{
    return (a * 3 + b) * 3 + c;
}

static void init(void)
{
    if (init_done) return;
    /* build V: triple must satisfy b ≥ a and c ≥ (a+b) % 3 */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                int idx = state_index(a, b, c);
                V[idx] = (b >= a) && (c >= (a + b) % 3) ? (seq_t)1 : (seq_t)0;
            }

    /* build adjacency lists */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                int i = state_index(a, b, c);
                int sum = (a + b + c) % 3;
                int cnt = 0;
                for (int d = 0; d < 3; ++d)
                    if (d >= sum) {
                        next_idx[i][cnt++] = state_index(b, c, d);
                    }
                next_cnt[i] = cnt;      /* cnt ∈ {0,1,2,3} */
            }

    init_done = true;
}

/* a(idx_t n) ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1; n<1 undefined */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    init();                          /* ensure V and adjacency are ready */

    if (n == 3) {                    /* sum of valid triples */
        seq_t total = 0;
        for (int i = 0; i < NSTATES; ++i) total += V[i];
        return total;                /* should be 12 */
    }

    /* DP arrays */
    seq_t dp_cur[NSTATES];
    seq_t dp_next[NSTATES];

    /* initialize dp_cur with V (states of length 3) */
    for (int i = 0; i < NSTATES; ++i) dp_cur[i] = V[i];

    /* iterate lengths from 4 up to n */
    for (idx_t len = 4; len <= n; ++len) {
        for (int i = 0; i < NSTATES; ++i) dp_next[i] = (seq_t)0;
        for (int i = 0; i < NSTATES; ++i) {
            if (dp_cur[i] == (seq_t)0) continue;
            seq_t val = dp_cur[i];
            int cnt = next_cnt[i];
            for (int t = 0; t < cnt; ++t) {
                int j = next_idx[i][t];
                dp_next[j] += val;
            }
        }
        /* move to next step */
        for (int i = 0; i < NSTATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all end states */
    seq_t total = 0;
    for (int i = 0; i < NSTATES; ++i) total += dp_cur[i];
    return total;
}
