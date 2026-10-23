/*
 * A161650  Number of reduced words of length n in the Weyl group A_37.
 * https://oeis.org/A161650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{37} (1 - x^(k+1))/(1 - x)
 *            which is equivalent to ∏_{k=1}^{37} (1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state holding the precalculated polynomial coefficients */
    static seq_t *poly = NULL;      /* coefficient array */
    static size_t  poly_len = 0;    /* number of stored coefficients (index+1) */
    static int initialized = 0;

    if (!initialized) {
        /* start with polynomial 1 */
        poly_len = 1;
        poly = (seq_t*)calloc(poly_len, sizeof(seq_t));
        poly[0] = (seq_t)1;

        /* multiply by (1 + x + … + x^k) for k = 1 .. 37 */
        for (int k = 1; k <= 37; ++k) {
            size_t new_len = poly_len + (size_t)k;
            seq_t *new_poly = (seq_t*)calloc(new_len, sizeof(seq_t));

            for (size_t i = 0; i < poly_len; ++i) {
                const seq_t val = poly[i];
                if (val == (seq_t)0) continue;
                for (int j = 0; j <= k; ++j) {
                    new_poly[i + (size_t)j] += val;
                }
            }

            free(poly);
            poly = new_poly;
            poly_len = new_len;
        }

        initialized = 1;
    }

    if (n >= (idx_t)poly_len)               /* out of range → 0 */
        return (seq_t)0;

    return poly[(size_t)n];
}
