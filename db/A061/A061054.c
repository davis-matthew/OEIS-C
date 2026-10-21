/*
 * A061054  a(n) = floor(n + n^(3/4)).
 * https://oeis.org/A061054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n + n^(3/4)⌋
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_n_pow_3_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the integer part of n^(3/4) by evaluating
   the fourth root of n^3, using only integer arithmetic.  The result is
   then added to n (ignoring the fractional part).  All intermediate
   computations are performed in 128‑bit integer to avoid overflow for
   the values expected in the test set. */
#include <stdint.h>
#include "../utils/floor_n_pow_3_4.h"

seq_t a(idx_t n)
{
    seq_t term = (seq_t)n + floor_n_pow_3_4(n);
    return term;
}
