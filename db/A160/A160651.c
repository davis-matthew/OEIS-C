/*
 * A160651  a(n) is the number of triangular nonnegative integers that are
 *          each equal to n(n+1)/2 - m(m+1)/2, for some m's where 0 <= m <= n.
 * https://oeis.org/A160651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A160651
 *
 * a(n) = number of m (0 ≤ m ≤ n) such that T_n - T_m is a triangular
 * number, where T_k = k(k+1)/2.
 *
 * Since a number x is triangular iff 8x+1 is a perfect square, the test
 * becomes: 8*(T_n - T_m)+1 is a perfect square.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Triangular number T_n */
    seq_t Tn = (seq_t)n * ((seq_t)n + 1) / 2;

    seq_t d = 0;        /* current difference T_n - T_m */
    seq_t count = 0;    /* result */

    for (idx_t m = n; ; --m) {
        /* For m == n, d stays 0. For the next iterations, 
           add (m+1) to get the new difference. */
        if (m < n)
            d += (seq_t)(m + 1);

        /* Check if d is triangular: 8*d + 1 must be a perfect square. */
        seq_t val = 8 * d + 1;
        seq_t r = floor_sqrt(val);
        if (r * r == val)
            ++count;

        if (m == 0)
            break;
    }

    return count;
}
