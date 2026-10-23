/*
 * A125563  Denominator of (4n+1)!/5^n.
 * https://oeis.org/A125563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 5^(max(0, n - Σ_{k≥1} ⌊(4n+1)/5^k⌋))
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent of 5 in (4n+1)! */
    idx_t m = 4*n + 1;
    idx_t v = 0;
    for (idx_t p = 5; p <= m; p *= 5) {
        v += m / p;
        if (p > m / 5) break;   /* prevent overflow of p*5 */
    }

    /* needed exponent in the denominator */
    idx_t d = (n > v) ? n - v : 0;

    /* compute 5^d */
    seq_t res = 1;
    for (idx_t i = 0; i < d; ++i) {
        res *= 5;
    }
    return res;
}
