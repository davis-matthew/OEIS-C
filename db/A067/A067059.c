/*
 * A067059  Square array read by antidiagonals of partitions which half fill
 *          an n*k box, i.e., partitions of floor(nk/2) or ceiling(nk/2) into
 *          up to n positive integers, each no more than k.
 * https://oeis.org/A067059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of floor(nk/2) into ≤ n parts, each
 *            part ≤ k, where n and k are obtained by reading the square array
 *            of these values antic-diagonally
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 128-bit types may be used for seq_t; idx_t is an unsigned integer type. */

#include <stdbool.h>

/* maximum values needed for indexes up to 10000 */
#define NMAX 141          /* maximum n  (approx sqrt(2*10000))  */
#define KMAX 141          /* maximum k  (same)                  */
#define MMAX ((NMAX * KMAX) / 2)   /* maximum m = floor(nk/2) */

/* The required function */
seq_t a(idx_t idx)
{
    /* static arrays to cache all values up to NMAX/KMAX */
    static bool ready = false;
    static seq_t val[NMAX + 1][KMAX + 1];

    if (!ready) {
        /* dp[n][m] – partitions of m using ≤ n parts, parts ≤ current k */
        static seq_t dp[NMAX + 1][MMAX + 1];

        /* initialise dp and val for k = 0 */
        for (int n = 0; n <= NMAX; ++n) {
            for (int m = 0; m <= MMAX; ++m) dp[n][m] = 0;
            dp[n][0] = 1;          /* zero sum has exactly one partition (empty) */
        }
        for (int n = 0; n <= NMAX; ++n) val[n][0] = 1;   /* floor(0*k/2) = 0 */

        /* iterate over part sizes k = 1 … KMAX */
        for (int k = 1; k <= KMAX; ++k) {
            for (int n = 1; n <= NMAX; ++n) {
                for (int m = k; m <= MMAX; ++m) {
                    /* add partitions that use at least one part of size k */
                    dp[n][m] += dp[n - 1][m - k];
                }
            }

            /* store the required coefficient for each n at this k */
            for (int n = 0; n <= NMAX; ++n) {
                int m_floor = (n * k) / 2;   /* floor(nk/2) */
                val[n][k] = dp[n][m_floor];
            }
        }
        ready = true;
    }

    /* map overall index to coordinates (n,k) on the antidiagonal array */
    if (idx == 0) return val[0][0];

    idx_t t = 0;
    while (((idx_t)(t + 1)) * ((idx_t)(t + 2)) / 2 <= idx) ++t;
    idx_t prev_sum = ((idx_t)t) * ((idx_t)(t + 1)) / 2;
    idx_t d = idx - prev_sum;          /* offset within diagonal t */
    idx_t n_part = d;
    idx_t k_part = t - d;

    /* array indices are small enough for int */
    return val[(int)n_part][(int)k_part];
}
