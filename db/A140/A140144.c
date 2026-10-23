/*
 * A140144  a(1)=1, a(n)=a(n-1)+n^1 if n odd, a(n)=a(n-1)+ n^0 if n is even.
 * https://oeis.org/A140144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)/2)^2 + (n/2)
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n / 2;
    idx_t m = (n + 1) / 2;
    return (seq_t)m * (seq_t)m + (seq_t)t;
}
