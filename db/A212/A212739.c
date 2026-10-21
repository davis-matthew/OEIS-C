/*
 * A212739  a(n) = 2^(n^2) - 1.
 * https://oeis.org/A212739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^2 as a seq_t to avoid overflow on intermediate types. */
    seq_t exp = (seq_t)n * (seq_t)n;

    /* Compute 2^(n^2).  big_pow is available for integer and arbitrary‑precision types. */
    seq_t val = big_pow((seq_t)2, exp);

    /* Subtract 1, cast to seq_t for safety. */
    return val - (seq_t)1;
}
