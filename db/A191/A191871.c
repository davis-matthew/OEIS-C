/*
 * A191871  a(n) = numerator(n^2 / 2^n).
 * https://oeis.org/A191871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (odd part of n)^2; equivalently a(2n)=a(n), a(2n+1)=(2n+1)^2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t u = n;
    while ((u & 1) == 0) u >>= 1;
    return (seq_t)u * (seq_t)u;
}
