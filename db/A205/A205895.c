/*
 * A205895  Number of 3X(n+1) 0..2 arrays with 2X2 subblock determinants
 *          differing from horizontal neighbors and permanents differing from
 *          vertical neighbors.
 * https://oeis.org/A205895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A205895 computed by dynamic programming over 3×(n+1)
 *            arrays of digits 0..2. States encode the last column and the two
 *            2×2 determinant values. Transitions enforce that each new 2×2
 *            determinant differs from its horizontal neighbour, and each
 *            vertical permanent differs from that of the cell...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Configuration constants (do not modify)                            */
/* ------------------------------------------------------------------ */
#define STATE_COUNT    (27 * 81)        /* 27 columns × 9 determinant values × 9 */
#define MAX_TRANS      (STATE_COUNT * 27)
#define MAX_N          10000            /* maximum n that can be queried */

/* ------------------------------------------------------------------ */
/* Static tables and state handling                                   */
/* ------------------------------------------------------------------ */
static int  col_dig[27][3];
static int  det_val[27][27][2];
static int  perm_val[27][27][2];
static bool vert_ok[27][27];

static unsigned char  ncnt[STATE_COUNT];
static unsigned int   noff[STATE_COUNT];
static unsigned short nbor[MAX_TRANS];

static seq_t dp0[STATE_COUNT];
static seq_t dp1[STATE_COUNT];
static seq_t *dp_prev = dp0;
static seq_t *dp_next = dp1;

static seq_t answers[MAX_N + 1];      /* answers[n-1] = a(n)      */
static idx_t  max_n_done = 0;

/* ------------------------------------------------------------------ */
/* Helper routines                                                    */
/* ------------------------------------------------------------------ */
static inline unsigned short encode_state(int col, int d0, int d1)
{
    return (unsigned short)(col * 81 + (d0 + 4) * 9 + (d1 + 4));
}

/* ------------------------------------------------------------------ */
/* Initialization: build all necessary tables                       */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* encode columns in base‑3 */
    for (int i = 0; i < 27; ++i) {
        int t = i;
        for (int r = 0; r < 3; ++r) {
            col_dig[i][r] = t % 3;
            t /= 3;
        }
    }

    /* pre‑compute determinants, permanents and vertical‑compatibility */
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 27; ++j) {
            for (int r = 0; r < 2; ++r) {
                int a0 = col_dig[i][r];
                int a1 = col_dig[i][r + 1];
                int b0 = col_dig[j][r];
                int b1 = col_dig[j][r + 1];
                det_val[i][j][r]  = a0 * b1 - a1 * b0;
                perm_val[i][j][r] = a0 * b1 + a1 * b0;
            }
            vert_ok[i][j] = perm_val[i][j][0] != perm_val[i][j][1];
        }
    }

    /* transition lists for each state */
    int trans_cnt = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int colL   = s / 81;
        int rem    = s % 81;
        int d0_last = rem / 9   - 4;
        int d1_last = rem % 9   - 4;
        noff[s] = (unsigned int)trans_cnt;

        for (int colR = 0; colR < 27; ++colR) {
            if (!vert_ok[colL][colR]) continue;
            int d0_new = det_val[colL][colR][0];
            int d1_new = det_val[colL][colR][1];
            if (d0_new == d0_last || d1_new == d1_last) continue;

            unsigned short ns = encode_state(colR, d0_new, d1_new);
            nbor[trans_cnt++] = ns;
        }
        ncnt[s] = (unsigned char)(trans_cnt - (int)noff[s]);
    }

    /* initial DP for n = 1 (one column pair) */
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;
    for (int c0 = 0; c0 < 27; ++c0) {
        for (int c1 = 0; c1 < 27; ++c1) {
            if (!vert_ok[c0][c1]) continue;
            int d0 = det_val[c0][c1][0];
            int d1 = det_val[c0][c1][1];
            unsigned short sid = encode_state(c1, d0, d1);
            dp_prev[sid] += 1;
        }
    }

    /* store a(1) */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += dp_prev[i];
    answers[0] = total;          /* a(1) -> answers[0] */
    max_n_done = 1;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of offset */

    /* first invocation */
    if (max_n_done == 0) init();

    /* already computed ? */
    if (n <= max_n_done) return answers[n - 1];

    /* extend DP up to requested n */
    for (idx_t cur = max_n_done; cur < n; ++cur) {
        /* clear next DP array */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* apply transitions */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp_prev[s];
            if (cnt == 0) continue;
            unsigned int off = noff[s];
            unsigned char len = ncnt[s];
            for (unsigned char k = 0; k < len; ++k) {
                unsigned short ns = nbor[off + k];
                dp_next[ns] += cnt;
            }
        }

        /* swap dp_prev <-> dp_next */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;

        /* sum up total configurations for this n */
        seq_t total = 0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp_prev[i];
        answers[cur] = total;   /* answers[cur] is a(cur+1) */
    }

    max_n_done = n;
    return answers[n - 1];
}
