/*
 * A168321  a(n) = n +6 - a(n-1), with a(1) = 0.
 * https://oeis.org/A168321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (n-1)/2 for odd n, n/2 + 7 for even n (equivalently a(n)
 *            = (13 + 15*(-1)^n + 2*n)/4)
 * Range:     u8 n <= 328, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t once to avoid repeated casts */
    seq_t nseq = (seq_t)n;
    if (n & 1)        /* n is odd */
        return (nseq - 1) / 2;
    else              /* n is even */
        return (nseq >> 1) + (seq_t)7;
}
