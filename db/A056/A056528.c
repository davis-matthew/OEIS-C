/*
 * A056528  Sum of digits of square of sum of digits of square.
 * https://oeis.org/A056528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit_sum( digit_sum( (n*n) )^2 )
 * Range:     u8 n <= 12, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A056528 by applying A004159 twice. */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t x = (seq_t)n;

    /* Compute n*n safely using a wider type to avoid 64‑bit overflow. */
    __uint128_t sq128 = ( __uint128_t )x * ( __uint128_t )x;
    seq_t sq = (seq_t)sq128;    /* cast back to seq_t – for 64/128 bit types this
                                   keeps the lower bits; for arbitrary‑precision
                                   types the constructor handles the value. */

    /* First application of A004159: digit sum of n^2. */
    seq_t s1 = digit_sum(sq);

    /* Square the first digit sum. */
    seq_t sq2 = s1 * s1;

    /* Second application of A004159: digit sum of the square. */
    seq_t res = digit_sum(sq2);

    return res;
}
