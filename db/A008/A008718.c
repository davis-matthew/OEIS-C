/*
 * A008718  Expansion of g.f.: (1+x^9)/((1-x)*(1-x^4)*(1-x^6)*(1-x^12)).
 * https://oeis.org/A008718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n^3 + 21*n^2 + 171*n + 864 - 3*n*((n^2+2*n) mod
 *            12) + 9*n*(((n+1) mod 4) - (n mod 4)))/864)
 * Range:     u8 n <= 53, u16 n <= 376, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit signed arithmetic – values stay < 2^63 for all n of interest */
    int64_t nn = (int64_t)n;
    int64_t n2 = nn * nn;
    int64_t n3 = n2 * nn;

    int64_t term1 = n3;
    int64_t term2 = 21 * n2;
    int64_t term3 = 171 * nn;
    int64_t term4 = 864;

    int64_t mod12 = (n2 + 2 * nn) % 12;
    int64_t sub1  = 3 * nn * mod12;

    int diff = (int)((n + 1) & 3) - (int)(n & 3);          /* (n+1 mod 4) - (n mod 4) */
    int64_t sub2 = 9 * nn * diff;

    int64_t numer = term1 + term2 + term3 + term4 - sub1 + sub2;
    seq_t result = (seq_t)(numer / 864);
    return result;
}
