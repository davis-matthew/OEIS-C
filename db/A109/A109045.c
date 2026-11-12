/*
 * A109045  a(n) = lcm(n, 4).
 * https://oeis.org/A109045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n/gcd(n, 4)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8939 (2026-11-12)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t g = 4, m = n % 4;
    while (m != 0) {            /* gcd(n mod 4, 4) */
        idx_t t = g % m;
        g = m;
        m = t;
    }
    return (seq_t)n * (seq_t)(4 / g);
}
