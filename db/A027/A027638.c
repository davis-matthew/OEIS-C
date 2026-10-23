/*
 * A027638  Order of 2^n X 2^n unitary group H_n acting on Siegel modular
 *          forms.
 * https://oeis.org/A027638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n^2+2*n+2) * \prod_{j=1..n} (4^j-1) (OEIS A027638)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

/* Computes the nth term of OEIS A027638. */
seq_t a(idx_t n)
{
    /* a(0) = 4 */
    if (n == 0)
        return (seq_t)4;

    seq_t result = (seq_t)4;          /* a(0) */
    seq_t pow4   = (seq_t)4;          /* 4^1   */
    seq_t pow2   = (seq_t)8;          /* 2^(2*1+1) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* term = (4^i - 1) * 2^(2*i+1) */
        seq_t term = seq_mul(pow4, (seq_t)1) - (seq_t)1;   /* 4^i - 1 */
        term = seq_mul(term, pow2);                       /* * 2^(2*i+1) */
        result = seq_mul(result, term);                    /* cumulative product */

        /* Update powers for next iteration */
        pow4 = seq_mul(pow4, (seq_t)4);   /* 4^(i+1) */
        pow2 = seq_mul(pow2, (seq_t)4);   /* 2^(2*(i+1)+1) */
    }

    return result;
}
