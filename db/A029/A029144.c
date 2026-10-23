/*
 * A029144  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A029144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3+51*n^2+390*n+1960)/2520 - (n mod 2)/8 +
 *            ((n+2) mod 3)/9)
 * Range:     u8 n <= 60, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* all intermediate calculations are done in 64‑bit unsigned type
       to avoid overflow for all supported seq_t sizes. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2 = nn * nn;
    unsigned long long n3 = n2 * nn;

    unsigned long long numerator0 = 2ULL * n3 + 51ULL * n2 + 390ULL * nn + 1960ULL;
    unsigned long long mod3 = (nn + 2ULL) % 3ULL;
    unsigned long long mod2 = nn % 2ULL;

    unsigned long long numerator_total =
        numerator0 + 280ULL * mod3 - 315ULL * mod2;

    unsigned long long result = numerator_total / 2520ULL;

    return (seq_t)result;
}
