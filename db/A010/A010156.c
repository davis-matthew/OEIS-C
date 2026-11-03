/*
 * A010156  Continued fraction for sqrt(78).
 * https://oeis.org/A010156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 8, 1, 4, 1, 16, 1, 4, 1, 16, 1, 4, 1, 16, 1, 4, 1,
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 89 terms through n = 9449 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Multiplicative rule: a(0)=8; for n≥1, let v2 be exponent of 2 in n.
 *   Then a(n) = 1 if v2=0, 4 if v2=1, 16 if v2≥2.  All odd prime powers contribute 1. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    if (n == 1)
        return (seq_t)1;

    idx_t v2 = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        v2++;
        m >>= 1;
    }

    if (v2 == 0)
        return (seq_t)1;
    if (v2 == 1)
        return (seq_t)4;
    return (seq_t)16;
}
