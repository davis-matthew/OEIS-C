/*
 * A232995  a(n) = (n - 3)^2*(n - 4)*2^n*n^(n-5).
 * https://oeis.org/A232995
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 3)^2 * (n - 4) * 2^n * n^(n-5)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For n < 5 the product contains a zero factor, hence the
       result is exactly 0. */
    if (n < 5)
        return 0;

    /* 2^n */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* n^(n-5) */
    seq_t pown = seq_pow((seq_t)n, n - 5);

    /* (n-3)^2 * (n-4) */
    seq_t factor = (seq_t)n - 3;
    factor *= factor;            /* (n-3)^2 */
    factor *= (seq_t)n - 4;      /* * (n-4) */

    /* Combine all parts */
    pow2 *= factor;
    pow2 *= pown;

    return pow2;
}
