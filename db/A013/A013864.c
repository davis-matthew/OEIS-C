/*
 * A013864  a(n) = 12^(5*n + 3).
 * https://oeis.org/A013864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Computes a(n) = 12^(5*n + 3) for any nonnegative n. */
seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 3 as a seq_t. */
    seq_t exponent = (seq_t)n * (seq_t)5 + (seq_t)3;
    /* Compute 12^exponent using the provided big_pow helper. */
    return big_pow((seq_t)12, exponent);
}
