/*
 * A029927  Convert n from nautical miles to statute miles.
 * https://oeis.org/A029927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((115078*n + 50000)/100000) (round to nearest
 *            integer)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 115078;
    t += 50000;
    return t / 100000;
}
