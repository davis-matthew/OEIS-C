/*
 * A106609  Numerator of n/(n+8).
 * https://oeis.org/A106609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, mult, easy
 * Formula:   a(n) = n/gcd(n,8)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    unsigned r = (unsigned)(n & 7ULL);   /* n mod 8 */
    seq_t g;
    switch (r)
    {
        case 0: g = 8; break;
        case 4: g = 4; break;
        case 2: case 6: g = 2; break;
        default: g = 1; break;
    }
    return val / g;
}
