/*
 * A029224  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( * (n^3 + 60*n^2 + 1044*n + 5184)/7920 * - (n^2 +
 *            29*n + 78)*(n mod 2)/240 * + ((2*n^3 + 10*n^2 + 9*n + 6) mod
 *            11)/11 * )
 * Range:     u8 n <= 107, u16 n <= 783, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit integer for all intermediate calculations
     * so that even when seq_t is 32 bits the computation does
     * not overflow.  The final result always fits in seq_t
     * for the ranges of n that appear in the test data. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* First term numerator (denominator 7920) */
    unsigned __int128 term1_num =
        nn * nn * nn          /* n^3          */
        + (unsigned __int128)60 * nn * nn
        + (unsigned __int128)1044 * nn
        + 5184;

    /* Second term: subtract ((n^2 + 29*n + 78)*(n mod 2))/240
     * Convert to same denominator 7920 by multiplying by 33. */
    unsigned __int128 term2_num =
        nn * nn + (unsigned __int128)29 * nn + 78;
    unsigned __int128 term2_scaled =
        ((n & 1) ? term2_num * 33 : 0);

    /* Third term: + ((2*n^3 + 10*n^2 + 9*n + 6) mod 11)/11
     * Convert to denominator 7920 by multiplying the remainder by 720. */
    unsigned __int128 m11 =
        ( (unsigned __int128)2 * nn * nn * nn
          + (unsigned __int128)10 * nn * nn
          + (unsigned __int128)9  * nn
          + 6 ) % 11;
    unsigned __int128 term3_scaled = m11 * 720;

    /* Combine all terms and take the floor division by 7920 */
    unsigned __int128 total = term1_num - term2_scaled + term3_scaled;
    unsigned long long result = (unsigned long long)(total / 7920ULL);

    return (seq_t)result;
}
