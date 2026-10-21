/*
 * A205021  Number of (n+1)X3 0..3 arrays with the determinants of 2X2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A205021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -102*a(n-2) -656*a(n-3) +4911*a(n-4)
 *            -9576*a(n-5) +5400*a(n-6) Recurrence initial values a(1)..a(21)
 *            are computed by dynamic programming. For n <= 21 the function
 *            returns the exact counts, for n > 21 it uses the recurrence
 *            above
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 64
#define DIAM 19
#define SLICE (DIAM * DIAM)
#define TOTAL ((ROWS) * SLICE)

/* helper to sum an array of seq_t of given length */
static seq_t sum_array(const seq_t *arr, size_t len)
{
    seq_t s = 0;
    for (size_t i = 0; i < len; ++i)
        s += arr[i];
    return s;
}

/* compute the first 21 terms of the sequence */
static void init_small_terms(seq_t *pre_out)
{
    /* structure holding a 2x2 determinant pair */
    struct Det {int d0, d1;};
    static struct Det det[ROWS][ROWS];
    static int det_init = 0;
    if (!det_init) {
        for (int r0 = 0; r0 < ROWS; ++r0) {
            int a0 = (r0 >> 4) & 3;
            int a1 = (r0 >> 2) & 3;
            int a2 = r0 & 3;
            for (int r1 = 0; r1 < ROWS; ++r1) {
                int b0 = (r1 >> 4) & 3;
                int b1 = (r1 >> 2) & 3;
                int b2 = r1 & 3;
                det[r0][r1].d0 = a0 * b1 - a1 * b0;
                det[r0][r1].d1 = a1 * b2 - a2 * b1;
            }
        }
        det_init = 1;
    }

    seq_t *dp = (seq_t *)calloc(TOTAL, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(TOTAL, sizeof(seq_t));

    /* base case: arrays with 2 rows (n = 1) */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int d0 = det[r0][r1].d0;
            int d1 = det[r0][r1].d1;
            if (d0 <= d1) {
                size_t idx = (size_t)r1 * SLICE
                            + (size_t)(d0 + 9) * DIAM
                            + (size_t)(d1 + 9);
                dp[idx] += 1;
            }
        }
    }

    pre_out[1] = sum_array(dp, TOTAL);

    /* temporary prefix array used for each previous row */
    static seq_t pref[DIAM][DIAM];

    /* build terms for n = 2 .. 21 (i.e. rows 3 .. 22) */
    for (idx_t n = 2; n <= 21; ++n) {
        /* compute contributions from all previous rows */
        for (int r1 = 0; r1 < ROWS; ++r1) {
            size_t base = (size_t)r1 * SLICE;

            /* 2‑D prefix sums of dp entries for previous row r1 */
            for (int a = 0; a < DIAM; ++a) {
                for (int b = 0; b < DIAM; ++b) {
                    size_t idx = base + ((size_t)a * DIAM + b);
                    seq_t val = dp[idx];
                    seq_t sum = val;
                    if (a > 0) sum += pref[a - 1][b];
                    if (b > 0) sum += pref[a][b - 1];
                    if (a > 0 && b > 0) sum -= pref[a - 1][b - 1];
                    pref[a][b] = sum;
                }
            }

            /* for each possible next row r2 combine prefix sums */
            for (int r2 = 0; r2 < ROWS; ++r2) {
                int d0 = det[r1][r2].d0;
                int d1 = det[r1][r2].d1;
                if (d0 <= d1) {
                    size_t aidx = (size_t)(d0 + 9);
                    size_t bidx = (size_t)(d1 + 9);
                    seq_t add = pref[aidx][bidx];
                    size_t new_idx = (size_t)r2 * SLICE + aidx * DIAM + bidx;
                    dp_next[new_idx] += add;
                }
            }
        }

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
        memset(dp_next, 0, TOTAL * sizeof(seq_t));

        pre_out[n] = sum_array(dp, TOTAL);
    }

    free(dp);
    free(dp_next);
}

/* public function to return a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    static seq_t pre[22];          /* indices 1..21 contain the first 21 terms */
    static int pre_init = 0;

    if (!pre_init) {
        init_small_terms(pre);
        pre_init = 1;
    }

    if (n <= 21)
        return pre[(size_t)n];

    /* recurrence: a(n) = 24*a(n-1) - 102*a(n-2) - 656*a(n-3)
                     + 4911*a(n-4) - 9576*a(n-5) + 5400*a(n-6) */
    seq_t state[6];  /* state[0] = a(n-6), state[5] = a(n-1) */
    state[0] = pre[(size_t)16];
    state[1] = pre[(size_t)17];
    state[2] = pre[(size_t)18];
    state[3] = pre[(size_t)19];
    state[4] = pre[(size_t)20];
    state[5] = pre[(size_t)21];

    for (idx_t k = 22; k <= n; ++k) {
        seq_t val = state[5] * ((seq_t)24)
                  - state[4] * ((seq_t)102)
                  - state[3] * ((seq_t)656)
                  + state[2] * ((seq_t)4911)
                  - state[1] * ((seq_t)9576)
                  + state[0] * ((seq_t)5400);

        /* shift window */
        state[0] = state[1];
        state[1] = state[2];
        state[2] = state[3];
        state[3] = state[4];
        state[4] = state[5];
        state[5] = val;
    }

    return state[5];
}
