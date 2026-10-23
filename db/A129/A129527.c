/*
 * A129527  a(2n) = a(n) + 2n, a(2n+1) = 2n + 1.
 * https://oeis.org/A129527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2*n) = a(n) + 2*n, a(2*n+1) = 2*n+1; a(0)=0
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n & 1)               /* n is odd: a(n) = n */
        return (seq_t)n;
    idx_t k = n >> 1;        /* n = 2*k */
    seq_t val = a(k);        /* a(2*k) = a(k) + 2*k */
    return val + (seq_t)(2 * k);
}
