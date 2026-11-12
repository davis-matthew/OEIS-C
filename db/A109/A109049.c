/*
 * A109049  a(n) = lcm(n, 8).
 * https://oeis.org/A109049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n/gcd(n, 8)
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9809 (2026-11-12)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t g = 8, m = n % 8;
    while (m != 0) {            /* gcd(n mod 8, 8) */
        idx_t t = g % m;
        g = m;
        m = t;
    }
    return (seq_t)n * (seq_t)(8 / g);
}
