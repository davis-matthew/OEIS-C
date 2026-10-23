/*
 * A166154  a(n) = 7*n*(n+1)/2 - 5.
 * https://oeis.org/A166154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*(n+1)/2 - 5
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = x * (x + 1) / 2;  /* triangular number T_n */
    return t * 7 - 5;
}
