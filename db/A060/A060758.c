/*
 * A060758  a(n) = 5^(n^2).
 * https://oeis.org/A060758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* exponent = n^2 (stored in seq_t to avoid overflow when seq_t is big) */
    seq_t exp = (seq_t)n * (seq_t)n;
    /* base = 5 as seq_t */
    seq_t base = 5;
    /* compute 5^exp */
    return pow_seq(base, (idx_t)exp);
}
