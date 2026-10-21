/*
 * A215864  Number of simple labeled graphs on n+4 nodes with exactly n
 *          connected components that are trees or cycles.
 * https://oeis.org/A215864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+4,5)*(15*n^3+390*n^2+2285*n+3886)/48
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 29, u64 n <= 534, u128 n <= 1000,
 *            big any n
 * Helpers:   binom, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* C(n+4,5) */
    seq_t coef = binom(n + 4, 5);

    /* polynomial part: 15*n^3 + 390*n^2 + 2285*n + 3886 */
    seq_t nn   = (seq_t)n;
    seq_t poly = (seq_t)15 * nn * nn * nn
               + (seq_t)390 * nn * nn
               + (seq_t)2285 * nn
               + 3886;

    /* divide by 48 using gcd cancellation to avoid intermediate overflow */
    seq_t denom = 48;
    seq_t g;

    while (denom > 1) {
        g = seq_gcd(coef, denom);
        if (g > 1) {
            coef /= g;
            denom /= g;
            continue;
        }
        g = seq_gcd(poly, denom);
        if (g > 1) {
            poly /= g;
            denom /= g;
            continue;
        }
        /* No further reduction possible – the division must succeed */
        break;
    }

    return coef * poly;
}
