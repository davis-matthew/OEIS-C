/*
 * A065739  Largest square <= sum of first n squares.
 * https://oeis.org/A065739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor_sqrt_seq( n*(n+1)*(2*n+1)/6 ) ^ 2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the helper that provides floor_sqrt_seq() */
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute S(n) = n(n+1)(2n+1)/6 in seq_t */
    seq_t s = nn * (nn + 1) * (2 * nn + 1);
    s /= 6;

    /* Find floor(sqrt(s)) using the provided helper */
    seq_t r = floor_sqrt_seq(s);

    /* Return the square of that integer */
    return r * r;
}
