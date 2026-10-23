/*
 * A155641  a(n) = 8^n-5^n+1^n.
 * https://oeis.org/A155641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 5^n + 1^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Sequence a(n) for OEIS A155641. */
seq_t a(idx_t n)
{
    seq_t p8 = big_pow((seq_t)8, (seq_t)n);
    seq_t p5 = big_pow((seq_t)5, (seq_t)n);
    seq_t result = p8 - p5 + (seq_t)1;
    return result;
}
