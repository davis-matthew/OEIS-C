/*
 * A019427  Continued fraction for tan(1/4).
 * https://oeis.org/A019427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=0, a(1)=3, a(n)=1 if n even (n>0), a(n)=8*((n-1)/2)+2 if n
 *            odd (n>1)
 * Range:     u8 n <= 64, u16 n <= 9351, u32 n <= 9351, u64 n <= 9351,
 *            u128 n <= 9351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 3;
    if ((n & 1u) == 0u) return 1;
    seq_t k = (seq_t)(n - 1) / 2;
    return 8 * k + 2;
}
