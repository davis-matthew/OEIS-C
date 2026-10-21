/*
 * A089264  Number of permutations of length n containing exactly once 132 and
 *          213, likewise for pattern pair (231,312).
 * https://oeis.org/A089264
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 21*n - 28) * 2^(n-9) for n ≥ 7 (for n = 4,5,6 the
 *            terms are 3, 6, 17 respectively)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset and small known values. */
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)6;
    if (n == 6) return (seq_t)17;

    /* For n ≥ 7 the closed formula applies. */
    if (n < 7) return (seq_t)0; /* behaviour for n<4 is irrelevant */

    /* Compute the polynomial part. */
    seq_t term = (seq_t)n * (seq_t)n + (seq_t)21 * n - (seq_t)28;

    /* Multiply or divide by 2^(n-9).  The exponent can be negative
       only for n == 7 or n == 8; in that case we divide by the
       corresponding power of two.  Use loops so that operations
       work for all supported seq_t types (including arbitrary
       precision integer classes). */
    if (n >= 9) {
        for (idx_t i = 0; i < n - 9; ++i)
            term = term * (seq_t)2;
    } else { /* 7 <= n < 9 */
        for (idx_t i = 0; i < 9 - n; ++i)
            term = term / (seq_t)2;
    }
    return term;
}
