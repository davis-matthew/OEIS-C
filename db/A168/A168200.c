/*
 * A168200  a(n) = 3*n - a(n-1) + 1, with a(1)=4.
 * https://oeis.org/A168200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*(n+1)/2) - (-1)^n
 * Range:     u8 n <= 168, u16 n <= 1100, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = floor(3*(n+1)/2) */
    seq_t base = ((seq_t)3 * (seq_t)(n + 1)) / (seq_t)2;

    /* adjust by ±1 depending on parity of n */
    if ((n & (idx_t)1) == 0)
        return base - (seq_t)1;
    else
        return base + (seq_t)1;
}
