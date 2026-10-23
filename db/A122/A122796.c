/*
 * A122796  Connell (3,5)-sum sequence (partial sums of the (3,5)-Connell
 *          sequence).
 * https://oeis.org/A122796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A122796 (Connell (3,5)-sum sequence)
 * Formula: a(n) = T(n-1) + S(n)   where
 *   T(k) = k(k+1)/2   (kth triangular number)
 *   S(n) = n^2 - 2*Σ_D
 * Σ_D = sum_{d <= n, d duplicate} (n - d + 1)
 * Duplicate indices d are given by:
 *   d_m = (5m^2 + 7m + 4) / 2   for m = 0,1,2,…
 * This yields the (3,5)-Connell sequence as partial sums.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* not in offset range */
        return (seq_t)0;

    /* T(n-1) = (n-1)*n/2 */
    seq_t triangular = (seq_t)(n - 1) * (seq_t)n / 2;

    seq_t sum_D = 0;
    if (n >= 2) {
        /* largest m such that d_m <= n   (m >= 0)   */
        /* Solve 5m^2 + 7m + 4 <= 2n  →  m = floor((sqrt(40n-31)-7)/10) */
        seq_t disc = (seq_t)40 * (seq_t)n - 31;
        seq_t sqrt_disc = floor_sqrt(disc);

        seq_t num = sqrt_disc - 7;          /* sqrt_disc >= 7 for n >= 2 */
        seq_t M = num / 10;                 /* M = m_max  (seq_t, can cast to idx_t later) */
        seq_t M_plus1 = M + 1;              /* number of duplicate indices up to n */

        /* Sum of m from 0..M  and of m^2 */
        seq_t sum1 = M * (M + 1) / 2;                      /* Σ m  */
        seq_t sum2 = M * (M + 1) * (2 * M + 1) / 6;        /* Σ m² */

        /* Σ d_m  = ½[5Σm² + 7Σm + 4(M+1)] */
        seq_t sum_n = (5 * sum2 + 7 * sum1 + 4 * M_plus1) / 2;

        seq_t np1 = (seq_t)n + 1;                          /* n+1   */
        sum_D = M_plus1 * np1 - sum_n;                    /* Σ_D   */
    }

    /* S(n) = n² - 2*Σ_D */
    seq_t S_n = (seq_t)n * (seq_t)n - 2 * sum_D;

    return triangular + S_n;        /* a(n) */
}
