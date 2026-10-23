/*
 * A152744  7 times pentagonal numbers: a(n) = 7*n*(3*n-1)/2.
 * https://oeis.org/A152744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*(3*n-1)/2
 * Range:     u8 n <= 5, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * 3 - 1;
    return nn * t * 7 / 2;
}
