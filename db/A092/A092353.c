/*
 * A092353  Expansion of (1+x^3)/((1-x)^2*(1-x^3)^2).
 * https://oeis.org/A092353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1}^{n+3} floor(i/3)^2
 * Range:     u8 n <= 16, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n + 3 */
    idx_t m = n + 3;
    seq_t sum = 0;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t k = i / 3;                 /* floor(i/3) */
        seq_t ksq = (seq_t)k * (seq_t)k; /* k^2 in seq_t */
        sum += ksq;
    }
    return sum;
}
