/*
 * A228510  a(n) =
 *          (128*n^4/25+14528*n^3/225+20344*n^2/75+661816*n/1575+168)*(n+6)!/n!.
 * https://oeis.org/A228510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            ((128*n^4/25+14528*n^3/225+20344*n^2/75+661816*n/1575+168)*(n+6)!/n!)
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 68, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use unsigned __int128 for safe intermediate arithmetic */
    unsigned __int128 sn = (unsigned __int128)n;

    /* Q(n) = 8064*n^4 + 101696*n^3 + 427224*n^2 + 661816*n + 264600 */
    unsigned __int128 Q = 8064*sn*sn*sn*sn
                        + 101696*sn*sn*sn
                        + 427224*sn*sn
                        + 661816*sn
                        + 264600;

    unsigned __int128 num = Q;          /* numerator so far                */
    unsigned long long den = 1575ULL;   /* denominator to cancel later     */

    /* Multiply by each factor (n+1)...(n+6) while cancelling factors of 1575 */
    for (idx_t k = 1; k <= 6; ++k) {
        num *= (sn + k);
        if (den != 1) {
            unsigned long long rem = (unsigned long long)(num % den);
            unsigned long long g = rem;
            unsigned long long b = den;
            while (b) {
                unsigned long long t = g % b;
                g = b;
                b = t;
            }
            if (g > 1) {
                num /= g;
                den /= g;
            }
        }
    }

    /* Final cancel if any divisor remains (the product is guaranteed integer) */
    if (den > 1)
        num /= den;

    return (seq_t)num;
}
