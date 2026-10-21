/*
 * A059977  a(n) = binomial(n+2, 2)^4.
 * https://oeis.org/A059977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)*(n+2)/2)^4 = binomial(n+2, 2)^4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 360, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = ((seq_t)(n + 1) * (n + 2)) / 2;
    seq_t t2 = t * t;
    return t2 * t2;   /* t^4 */
}
