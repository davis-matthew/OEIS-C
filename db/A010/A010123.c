/*
 * A010123  Continued fraction for sqrt(14).
 * https://oeis.org/A010123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 3; for n > 0, a(n) = 1 + floor((n+2)/4) - floor((n+1)/4)
 *            + 5*(floor((n+4)/4) - floor((n+3)/4))
 * Range:     u8 n <= 9931, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)3;
    /* For n > 0 we use the 4‑periodic pattern: 1, 2, 1, 6 */
    idx_t k = (n - 1ULL) & 3ULL;   /* (n-1) mod 4, using bit mask since 4 is a power of two */
    switch (k) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)1;
        case 3: return (seq_t)6;
    }
    /* unreachable, but return 0 to satisfy compiler */
    return (seq_t)0;
}
