/*
 * A171470  a(n) = 6*a(n-1) - 8*a(n-2) for n > 2; a(0) = 11, a(1) = 90, a(2) =
 *          372.
 * https://oeis.org/A171470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 11; for n > 0, a(n) = 24·4ⁿ – 3·2ⁿ (recurrence a(n) =
 *            6·a(n‑1) – 8·a(n‑2) for n > 2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;

    /* Compute 4ⁿ and 2ⁿ using the utility pow routine. */
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* a(n) = 24 * 4ⁿ – 3 * 2ⁿ.  All operations are legal on seq_t. */
    seq_t result = (seq_t)24 * pow4;
    result -= (seq_t)3 * pow2;

    return result;
}
