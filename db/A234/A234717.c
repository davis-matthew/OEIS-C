/*
 * A234717  a(n) = floor(n/(exp(1/(2*n))-1)).
 * https://oeis.org/A234717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(8*n^2 - 2*n - 1 + (-1)^n)
 * Range:     u8 n <= 5, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t val = (seq_t)8 * n0 * n0 - (seq_t)2 * n0 - (seq_t)1;
    if ((n & 1) == 0) {
        val += 1;
    } else {
        val -= 1;
    }
    return val / 4;
}
