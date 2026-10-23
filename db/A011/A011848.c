/*
 * A011848  a(n) = floor(binomial(n, 2)/2).
 * https://oeis.org/A011848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor(n*(n-1)/4)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n*(n-1))/4 using unsigned arithmetic.
       The division truncates toward zero, which is exactly
       the floor for non‑negative integers. */
    return ((seq_t)n * ((seq_t)n - 1)) / 4;
}
