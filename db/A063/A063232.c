/*
 * A063232  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          77 ).
 * https://oeis.org/A063232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n - 5 + (-1)^n for n > 1; a(1) = 5
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;
    seq_t tn = (seq_t)n;
    seq_t val = (seq_t)10 * tn - (seq_t)5;
    if ((n & 1) == 0)
        val += (seq_t)1;
    else
        val -= (seq_t)1;
    return val;
}
