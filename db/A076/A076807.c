/*
 * A076807  Number of n X n matrices over an alphabet of size 12.
 * https://oeis.org/A076807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute the exponent as seq_t first to avoid overflow in the
       intermediate multiplication, then cast to idx_t for the power
       routine.  For the bounds considered (n ≤ 10000) the exponent
       fits comfortably in a 64‑bit unsigned integer. */
    seq_t exp_seq = (seq_t)n * (seq_t)n;
    idx_t exp = (idx_t)exp_seq;
    return pow_int(12, exp);
}
