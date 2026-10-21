/*
 * A213458  Number of 0..3 arrays of length n+3 with sum less than 6 in any
 *          length 4 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n+3 over alphabet {0,1,2,3}
 *            such that every consecutive 4 elements have sum < 6
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* DP over the last 3 digits.  There are 4^3 = 64 possible states,
   each state is encoded as a 6‑bit integer: (b1,b2,b3) with
   0 ≤ bi ≤ 3. */
static seq_t *a_vals = NULL;   /* memoisation table, a_vals[i] = a(i) */
static size_t a_n_max = 0;     /* largest n for which a_vals[n] is valid */

static seq_t prev[64], next[64];   /* DP buffers */
static int b1[64], b2[64], b3[64];
static int sum3[64];                /* pre‑computed b1+b2+b3 */

static bool initialized = false;   /* one‑time initialisation */

seq_t a(idx_t n)
{
    /* --- initialise static data -------------------------------------- */
    if (!initialized) {
        for (int s = 0; s < 64; ++s) {
            prev[s] = 1;              /* all 3‑tuples are allowed initially */
            b1[s] = (s >> 4) & 3;
            b2[s] = (s >> 2) & 3;
            b3[s] = s & 3;
            sum3[s] = b1[s] + b2[s] + b3[s];
        }
        a_vals = (seq_t *)malloc(sizeof(seq_t));
        a_vals[0] = 0;                 /* not used (offset is 1) */
        a_n_max = 0;
        initialized = true;
    }

    /* --- extend memoisation table to the required n ----------------- */
    if ((size_t)n > a_n_max) {
        size_t new_size = (size_t)n + 1;
        a_vals = (seq_t *)realloc(a_vals, new_size * sizeof(seq_t));

        for (size_t step = a_n_max + 1; step <= (size_t)n; ++step) {
            /* clear next buffer */
            for (int s = 0; s < 64; ++s) next[s] = 0;

            /* one DP transition from prev to next */
            for (int s = 0; s < 64; ++s) {
                seq_t cnt = prev[s];
                int s3 = sum3[s];
                for (int x = 0; x < 4; ++x) {
                    if (s3 + x < 6) {
                        int ns = (b2[s] << 4) | (b3[s] << 2) | x;
                        next[ns] += cnt;
                    }
                }
            }

            /* compute total number of arrays of length 3 + step */
            seq_t total = 0;
            for (int s = 0; s < 64; ++s) total += next[s];
            a_vals[step] = total;

            /* prepare for next iteration */
            for (int s = 0; s < 64; ++s) prev[s] = next[s];
        }

        a_n_max = (size_t)n;
    }

    return a_vals[(size_t)n];
}
