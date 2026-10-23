/*
 * A100256  Squares of second pentagonal numbers: a(n) = (1/4)*n^2*(3*n+1)^2.
 * https://oeis.org/A100256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(3*n+1)^2/4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t t   = 3*n_t + 1;
    if ((n_t & 1) == 0) {
        seq_t k = n_t >> 1;
        return (k * k) * (t * t);
    } else {
        seq_t m = t >> 1;
        return (n_t * n_t) * (m * m);
    }
}
