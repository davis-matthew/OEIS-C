/*
 * A153152  Rotated binary incrementing: For n<2 a(n)=n, if n=(2^k)-1,
 *          a(n)=(n+1)/2, otherwise a(n)=n+1.
 * https://oeis.org/A153152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n<2, a(n)=n; if n=(2^k)-1, a(n)=(n+1)/2; otherwise a(n)=n+1
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;
    if ((n & (n + 1)) == 0)
        return ((seq_t)n >> 1) + 1;
    return (seq_t)n + 1;
}
