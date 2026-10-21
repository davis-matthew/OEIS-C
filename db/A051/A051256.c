/*
 * A051256  Numbers formed from binomial coefficients (mod 2) interpreted as
 *          digits in factorial base.
 * https://oeis.org/A051256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = Sum_{k=0..n} (k+1)! * (C(n, k) mod 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1; /* 0! */

    for (idx_t i = 0; i <= n; ++i)
    {
        fact = fact * (seq_t)(i + 1);   /* (i+1)! */
        if ((i & ~n) == 0)
            sum += fact;
    }
    return sum;
}
