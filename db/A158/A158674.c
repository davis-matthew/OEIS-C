/*
 * A158674  Period 18: repeat [3, 4, 0, 8, 6, 3, 3, 7, 0, 2, 6, 6, 3, 1, 0, 5,
 *          6, 0].
 * https://oeis.org/A158674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = A106833(n) mod 9. A106833(n) is the polynomial: 3 + 4n +
 *            8n^3 + 6n^4 + 3n^5 + 3n^6 + 7n^7 + 2n^9 + 6n^10 + 6n^11 + 3n^12
 *            + n^13 + 5n^15 + 6n^16 Modulo 9 the values become periodic with
 *            period 18
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build the coefficients of the numerator polynomial once. */
    static int p[18] = {0};
    static char initialized = 0;

    if (!initialized) {
        /* The polynomial terms (coeff, exponent).  Coefficients are gathered
           from the generating function for A158674. */
        static const struct {int coeff; int exp;} terms[] = {
            {3,0},{4,1},{8,3},{6,4},{3,5},{3,6},{7,7},
            {2,9},{6,10},{6,11},{3,12},{1,13},{5,15},{6,16}
        };
        size_t i;
        for (i = 0; i < sizeof(terms)/sizeof(terms[0]); ++i) {
            p[terms[i].exp] = terms[i].coeff;
        }
        /* The value for n = 17 (i.e. a[18]) is 0 by definition; it remains
           zero because the numerator polynomial has degree 16. */
        initialized = 1;
    }

    /* The sequence is periodic with period 18.  n is 1‑based. */
    idx_t r = (n - 1) % 18;          /* 0 <= r < 18 */
    int val = p[r] % 9;              /* ensure value is in 0..8 */
    return (seq_t)val;
}
