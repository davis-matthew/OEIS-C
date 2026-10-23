/*
 * A125577  a(0) = 1; for n >= 1, a(n) = n^2 - a(n-1).
 * https://oeis.org/A125577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 + (-1)^n
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * ((seq_t)n + 1) / 2;
    return (n & 1) ? base - 1 : base + 1;
}
