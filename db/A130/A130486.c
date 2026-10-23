/*
 * A130486  a(n) = Sum_{k=0..n} (k mod 8) (Partial sums of A010877).
 * https://oeis.org/A130486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*floor(n/8) + (n mod 8)*(n mod 8 + 1)/2
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 8;            /* integer division, floor(n/8) */
    idx_t m = n % 8;            /* remainder modulo 8          */
    return (seq_t)28 * (seq_t)k + (seq_t)m * ((seq_t)m + 1) / 2;
}
