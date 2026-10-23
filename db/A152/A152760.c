/*
 * A152760  4 times 9-gonal numbers: a(n) = 2*n*(7*n-5).
 * https://oeis.org/A152760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(7*n-5)
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* a(n) = 14*n^2 - 10*n = n * (14*n - 10) */
    return nn * (((seq_t)14 * nn) - (seq_t)10);
}
