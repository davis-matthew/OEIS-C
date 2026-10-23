/*
 * A027379  Expansion of (1+x^2-x^3)/(1-x)^3.
 * https://oeis.org/A027379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+5)/2
 * Range:     u8 n <= 20, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */
    seq_t nn = (seq_t)n;
    return nn * (nn + 5) / 2;         /* a(n) for n ≥ 1 */
}
