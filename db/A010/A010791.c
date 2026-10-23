/*
 * A010791  a(n) = n!*(n+2)!/2.
 * https://oeis.org/A010791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n+2)!/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)i * (seq_t)i;         /* multiply by i² */
    }
    res *= (seq_t)(n + 1) * (seq_t)(n + 2); /* extra factors from (n+2)! */
    res /= 2;                               /* divide by 2 */
    return res;
}
