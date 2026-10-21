/*
 * A068158  a(n) = floor(n!/R(n)!), where R(n) = digit reversal of n
 *          (A004086).
 * https://oeis.org/A068158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(n! / R(n)!) where R(n) is the decimal reversal of n
 *            (OEIS A068158). If n < R(n) then a(n) = 0; if n = R(n) then a(n)
 *            = 1. Otherwise a(n) = product(i from R(n)+1 to n) i
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 39,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"

seq_t a(idx_t n)
{
    idx_t r = rev(n);            /* digit reversal of n */
    if (n < r) return 0;         /* 0 when n < R(n) */
    if (n == r) return 1;        /* 1 when n is a palindrome */

    seq_t prod = 1;
    for (idx_t i = r + 1; i <= n; ++i)
        prod *= (seq_t)i;        /* multiply using seq_t arithmetic */
    return prod;
}
