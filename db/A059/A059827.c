/*
 * A059827  Cubes of triangular numbers: (n*(n+1)/2)^3.
 * https://oeis.org/A059827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/2)^3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (t + 1) / 2;
    return t * t * t;
}
