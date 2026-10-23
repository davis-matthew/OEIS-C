/*
 * A132686  a(n) = binomial(2^n + 2*n + 1, n).
 * https://oeis.org/A132686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + 2*n + 1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* N = 2^n + 2*n + 1 */
    seq_t N = pow2(n) + ((seq_t)2 * (seq_t)n) + (seq_t)1;
    seq_t res = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t numerator = N - (seq_t)n + (seq_t)i;
        idx_t denom = i;

        /* cancel common factors between res and denom */
        seq_t g1 = gcd(res, (seq_t)denom);
        if (g1 != (seq_t)1) {
            res /= g1;
            denom /= (idx_t)g1;
        }

        /* cancel common factors between numerator and remaining denom */
        seq_t g2 = gcd(numerator, (seq_t)denom);
        if (g2 != (seq_t)1) {
            numerator /= g2;
            denom /= (idx_t)g2;
        }

        /* multiply and divide, denom should now be 1 */
        res = res * numerator;
        if (denom != (idx_t)1) {
            res /= (seq_t)denom;
        }
    }
    return res;
}
