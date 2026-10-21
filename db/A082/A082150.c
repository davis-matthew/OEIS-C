/*
 * A082150  A transform of C(n,2).
 * https://oeis.org/A082150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(2^(n-2)+1)*(n-1)*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases first.  For n = 0 or 1 the product
       (n-1)*n is zero, so the whole term is zero. */
    if (n <= 1)
        return 0;

    /* Compute (n-1)*n as a seq_t value. */
    seq_t prod = (seq_t)(n - 1) * (seq_t)n;

    /* 2^(n-2) – this is defined for n >= 2 because we skip the
       n<=1 case above. */
    seq_t pow_n_minus_2 = pow2(n - 2);

    /* (2^(n-2)+1) */
    seq_t term = pow_n_minus_2 + 1;

    /* Multiply the three integer factors together. */
    seq_t val = prod * term;

    /* Apply the remaining power of two:
       * if n < 4, we need to divide by 2^(4-n)
       * if n >= 4, we multiply by 2^(n-4)            */
    if (n < 4)
        val >>= (4 - n);      /* division by 2^(4-n) */
    else
        val <<= (n - 4);      /* multiplication by 2^(n-4) */

    return val;
}
