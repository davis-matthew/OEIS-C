/*
 * A107399  a(n) = binomial(n+8,8)*binomial(n+10,8).
 * https://oeis.org/A107399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+8,8)*binomial(n+10,8)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 54, u128 n <= 958,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+8, 8) and C(n+10, 8) and multiply them. */
    seq_t first  = binomial(n + 8, 8);
    seq_t second = binomial(n + 10, 8);
    return first * second;
}
