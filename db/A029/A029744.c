/*
 * A029744  Numbers of the form 2^k or 3*2^k.
 * https://oeis.org/A029744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for all m>=1: a(2m) = 2^m a(2m+1) = 3 * 2^(m-1) (so
 *            a(3)=3, a(5)=6, a(7)=12, …)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 1)
        return (seq_t)1;

    /* Even indices: a(2m) = 2^m */
    if ((n & 1ULL) == 0)
        return pow2(n / 2);

    /* Odd indices (n >= 3): a(2m+1) = 3 * 2^(m-1) */
    idx_t m = (n - 1) / 2;        /* m >= 1 for n >= 3 */
    seq_t val = pow2(m - 1);      /* 2^(m-1) */
    return val * (seq_t)3;        /* multiply by 3 */
}
