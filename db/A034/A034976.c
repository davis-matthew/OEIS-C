/*
 * A034976  One eighth of octo-factorial numbers.
 * https://oeis.org/A034976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^(n-1) * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The OEIS sequence A034976 satisfies 8·a(n) = 8^n · n!,
       so a(n) = 8^(n−1) · n!. */
    if (n == 0)
        return 0;                      /* out of range (offset = 1) */

    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;                /* compute n! first */

    seq_t pow8 = pow2(3 * (n - 1));     /* 8^(n−1) = 2^(3(n−1)) */
    res *= pow8;                       /* multiply to get a(n) */

    return res;
}
