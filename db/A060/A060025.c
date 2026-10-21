/*
 * A060025  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 6.
 * https://oeis.org/A060025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = P(n+5,5) - P(n+5,6) where P(k,m) is the number of
 *            partitions of k into exactly m parts. The recurrence used is:
 *            P(0,0) = 1, P(k,0) = 0 (k>0) P(k,m) = P(k-1,m-1) + P(k-m,m)
 *            (k≥1, m≥1) This computes all values for n up to ≈20000 in O(n)
 *            time and space
 * Range:     i8 n <= 24, i16 n <= 74, i32 n <= 710, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_K 20000   /* maximal k = n+5 handled (supports n up to about 19995) */

static seq_t p[MAX_K + 1][7];    /* p[k][m] : partitions of k into exactly m parts */
static int initialized = 0;

seq_t a(idx_t n)
{
    /* Ensure DP table is filled on the first call */
    if (!initialized) {
        /* initial conditions */
        p[0][0] = 1;
        for (idx_t m = 1; m <= 6; ++m)
            p[0][m] = 0;

        /* DP recurrence */
        for (idx_t k = 1; k <= MAX_K; ++k) {
            p[k][0] = 0;
            for (idx_t m = 1; m <= 6; ++m) {
                seq_t val = 0;
                if (k >= 1 && m-1 <= k-1)
                    val += p[k-1][m-1];
                if (k >= m)
                    val += p[k-m][m];
                p[k][m] = val;
            }
        }
        initialized = 1;
    }

    /* Compute a(n) using the precomputed table */
    idx_t k = n + 5;
    /* If k exceeds the precomputed range, the function cannot guarantee correctness.
       The problem statement limits n to about 10000, so k will always be ≤ MAX_K. */
    seq_t val5 = p[k][5];
    seq_t val6 = p[k][6];
    return val5 - val6;
}
