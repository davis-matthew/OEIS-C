/*
 * A161438  Number of reduced words of length n in the Weyl group A_6.
 * https://oeis.org/A161438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{6} (1 - x^(k+1))/(1 - x) =
 *            coefficient of x^n in ∏_{k=1}^{6} (1 + x + … + x^k). The
 *            polynomial is of degree 21; a(n)=0 for n>21
 * Range:     u8 n <= 5, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Number of terms in polynomial (degree 21 + 1) */
    enum { MAX_DEG = 21 };

    /* static memoised coefficients */
    static seq_t coeff[MAX_DEG + 1] = {0};
    static int initialized = 0;
    if (!initialized) {
        /* compute coefficients once */
        /* start with polynomial P(x)=1 (coeff[0]=1) */
        coeff[0] = 1;
        size_t cur_len = 1;          /* current highest degree + 1 */

        /* multiply by (1 + x + ... + x^k) for k = 1..6 */
        for (int k = 1; k <= 6; ++k) {
            size_t next_len = cur_len + k;   /* new degree + 1 */
            /* temporary array for next coefficients */
            seq_t next[MAX_DEG + 1] = {0};

            for (size_t i = 0; i < cur_len; ++i) {
                seq_t val = coeff[i];
                for (int t = 0; t <= k; ++t) {
                    next[i + t] += val;
                }
            }

            /* copy back to coeff and update length */
            for (size_t i = 0; i < next_len; ++i)
                coeff[i] = next[i];
            cur_len = next_len;
        }
        initialized = 1;
    }

    if (n > MAX_DEG)
        return (seq_t)0;
    else
        return coeff[(size_t)n];
}
