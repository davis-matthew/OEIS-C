/*
 * A217562  Even numbers not divisible by 5.
 * https://oeis.org/A217562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n + floor((n-1)/4))
 * Range:     u8 n <= 102, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor((n-1)/4) using unsigned division */
    idx_t q = (n - 1) / 4;
    /* convert to seq_t and apply the formula */
    seq_t result = (seq_t)n + (seq_t)q;
    return (seq_t)2 * result;
}
