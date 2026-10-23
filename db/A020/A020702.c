/*
 * A020702  Expansion of (1+x^10)/((1-x)*(1-x^2)*(1-x^3)*(1-x^5)).
 * https://oeis.org/A020702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((146*n^3+39*n^2+276*n+180)/180) -
 *            floor((4*n^3+n^2+4*n+2)/5)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t1 = 146 * N * N * N + 39 * N * N + 276 * N + 180;
    seq_t t2 = 4 * N * N * N + N * N + 4 * N + 2;
    return t1 / 180 - t2 / 5;
}
