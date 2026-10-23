/*
 * A118016  Integers of the form 2^k/(k-1).
 * https://oeis.org/A118016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^(n-1) - n + 2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n < 1 return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Compute 2^(n-1).  
       To avoid undefined behaviour we check that the shift amount
       does not exceed the storage width of seq_t. */
    unsigned width = sizeof(seq_t) * 8U;
    if (n - 1 >= width)
        return (seq_t)0;          /* overflow – value would not fit */

    seq_t pow2_n_minus_1 = ((seq_t)1) << (n - 1);

    /* Exponent: 2^(n-1) - n + 2. */
    seq_t exponent = pow2_n_minus_1 - (seq_t)n + (seq_t)2;

    /* Result: 2^exponent.  Use big_pow to accommodate arbitrarily
       large exponents that still fit in seq_t. */
    return big_pow((seq_t)2, exponent);
}
