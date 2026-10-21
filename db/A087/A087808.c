/*
 * A087808  a(0) = 0; a(2n) = 2a(n), a(2n+1) = a(n) + 1.
 * https://oeis.org/A087808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0) = 0; a(2n) = 2*a(n); a(2n+1) = a(n)+1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if ((n & 1ULL) == 0)
        return (seq_t)2 * a(n >> 1);
    else
        return a(n >> 1) + (seq_t)1;
}
