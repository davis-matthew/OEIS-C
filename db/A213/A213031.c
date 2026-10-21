/*
 * A213031  a(n) = floor(n/2)^3 - floor(n/3)^3.
 * https://oeis.org/A213031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2)^3 - floor(n/3)^3
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_half = (seq_t)n / 2;
    seq_t b_third = (seq_t)n / 3;
    seq_t a_cube = a_half * a_half * a_half;
    seq_t b_cube = b_third * b_third * b_third;
    return a_cube - b_cube;
}
