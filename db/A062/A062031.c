/*
 * A062031  Group odd numbers into (1), (3,5,7), (9,11,13,15,17), ...; a(n) =
 *          product of n-th group.
 * https://oeis.org/A062031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0}^{2*n-2} ( 2*k + 2*n*(n-2) + 3 )
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the constant part 2*n*(n-2)+3 safely */
    int64_t nn = (int64_t)n;                     /* n ≥ 1, fits in 64-bit */
    int64_t const_val = 2 * nn * (nn - 2) + 3;   /* always ≥ 1 for n ≥ 1 */
    seq_t C = (seq_t)const_val;

    seq_t prod = 1;
    /* number of factors is 2*n-1 (k = 0 .. 2*n-2) */
    for (idx_t k = 0; k < 2 * n - 1; ++k)
    {
        prod *= C + (seq_t)(2 * k);
    }
    return prod;
}
