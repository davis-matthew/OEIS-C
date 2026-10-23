/*
 * A013812  a(n) = 20^(4*n + 1).
 * https://oeis.org/A013812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent: exp = 4*n + 1 */
    seq_t exp = ((seq_t)4 * (seq_t)n) + (seq_t)1;

    /* Base 20 as seq_t */
    seq_t base = (seq_t)20;

    /* Return base ^ exp using the provided helper */
    return big_pow(base, exp);
}
