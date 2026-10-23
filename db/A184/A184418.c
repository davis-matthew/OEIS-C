/*
 * A184418  Convolution square of A040001.
 * https://oeis.org/A184418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = 5*k, a(2*k+1) = 4*k+2; a(0) = 1
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t k = n >> 1;                /* k = n/2 */
    if ((n & 1) == 0)                /* n even: a = 5*k */
        return (seq_t)5 * (seq_t)k;
    else                              /* n odd : a = 4*k + 2 */
        return (seq_t)4 * (seq_t)k + (seq_t)2;
}
