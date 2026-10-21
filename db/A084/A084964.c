/*
 * A084964  Follow n+2 by n. Also solution of a(n+2)=a(n)+1, a(0)=2, a(1)=0.
 * https://oeis.org/A084964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + 2 if n even, floor(n/2) if n odd
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = (seq_t)(n / 2);
    return half + ((n & 1) ? (seq_t)0 : (seq_t)2);
}
