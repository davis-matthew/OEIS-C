/*
 * A075351  a(n) = floor(2*binomial(n+1,2)!/(binomial(n,2)!*n*(n^2+1))).
 * https://oeis.org/A075351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*binomial(n+1,2)!/(binomial(n,2)!*n*(n^2+1)))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Works for any seq_t: unsigned int, uint8_t …, unsigned __int128, or arbitrary-precision integers */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; below that we return 0. */
    if (n < 1) return 0;

    /* a = C(n,2) = n*(n-1)/2,  b = C(n+1,2) = n*(n+1)/2 */
    seq_t a_tri = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Denominator = n*(n^2+1) */
    seq_t denom = (seq_t)n * (((seq_t)n * (seq_t)n) + 1);

    seq_t num = 2;  /* start with the factor 2 */

    for (idx_t i = 1; i <= n; ++i) {
        /* term = a_tri + i  (runs from a_tri+1 to b = a_tri+n) */
        seq_t term = a_tri + (seq_t)i;

        /* Remove common factors between term and denom to keep numbers small */
        seq_t g = gcd(term, denom);
        if (g > 1) {
            term /= g;
            denom /= g;
        }

        num *= term;
    }

    /* At this point denom divides num exactly */
    return num / denom;
}
