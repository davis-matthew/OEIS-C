/*
 * A111080  Sum of numbers under a triangle on a spiral staircase of width 10.
 * https://oeis.org/A111080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*(n+1)*(n+2)*(n+3)/6
 * Range:     u8 n <= 0, u16 n <= 16, u32 n <= 730, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn + 1;
    term *= nn + 2;
    term *= nn + 3;
    term *= (seq_t)11;
    term /= (seq_t)6;
    return term;
}
