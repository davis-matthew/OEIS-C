/*
 * A211703  a(n) = n + [n/2] + [n/3] + [n/4], where [] = floor.
 * https://oeis.org/A211703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n/2⌋ + ⌊n/3⌋ + ⌊n/4⌋
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The calculation is performed in the native idx_t type to avoid
       overflow until the final cast to seq_t. */
    idx_t sum = n + n/2 + n/3 + n/4;
    return (seq_t)sum;
}
