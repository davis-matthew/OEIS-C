/*
 * A085740  a(n) = T(n)^2 - n^2, where T(n) is a triangular number.
 * https://oeis.org/A085740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n)^2 - n^2, where T(n) = n*(n+1)/2 (triangular number)
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the direct computation:  a(n) = n^2 * (n^2 + 2*n - 3) / 4  */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* cast index to sequence type */
    seq_t t  = sn * (sn + 1) / 2;  /* T(n) = n*(n+1)/2 */
    return t * t - sn * sn;        /* a(n) = T(n)^2 - n^2 */
}
