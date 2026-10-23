/*
 * A165157  Zero followed by partial sums of A133622.
 * https://oeis.org/A165157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = k*(k+5)/2 ; a(2k+1) = k*(k+5)/2 + 1
 * Range:     u8 n <= 41, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t k = n >> 1;
    seq_t kk = (seq_t)k;
    seq_t val = (kk * (kk + (seq_t)5)) / (seq_t)2;
    return (n & 1) ? val + (seq_t)1 : val;
}
