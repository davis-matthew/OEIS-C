/*
 * A143314  Number of hands of n cards containing a straight flush (for n=1 to
 *          52).
 * https://oeis.org/A143314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = C(52,n) - coeff_n( ( (1+x)^13 - x^5(1+x) * (10 + 61x +
 *            156x^2 + 215x^3 + 169x^4 + 65x^5 + 12x^6 + x^7) )^4 )
 * Range:     u8 n <= 3, u16 n <= 2, u32 n <= 7, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The generating function for a single suit that avoids a straight flush
 * is f(x) = (1+x)^13 – x^5(1+x)P(x) with
 * P(x) = 10 + 61x + 156x^2 + 215x^3 + 169x^4 + 65x^5 + 12x^6 + x^7.
 * The number of 52‑card hands that avoid a straight flush equals the
 * coefficient of f(x)^4; hence a(n) = C(52,n) – coefficient.
 */

#include <stdbool.h>
#include "../utils/binom.h"

static bool ready = false;
static seq_t f4[53];

/*======================================================================
 * a(idx_t n)  – number of n‑card hands containing at least one straight flush
 *======================================================================*/
seq_t a(idx_t n)
{
    if (!ready) {
        /* f(x) coefficients (degree 0 … 13) */
        seq_t f[14] = {0};
        for (idx_t k = 0; k <= 13; ++k)
            f[k] = binom(13, k);

        /* subtract x^5(1+x)P(x) */
        static const seq_t P[8] = {10, 61, 156, 215, 169, 65, 12, 1};
        seq_t sub[14] = {0};
        for (idx_t i = 0; i < 8; ++i) {
            for (idx_t j = 0; j < 2; ++j) {           /* (1 + x) */
                idx_t exp = 5 + i + j;                /* degree of term */
                sub[exp] += P[i];
            }
        }
        for (idx_t k = 0; k <= 13; ++k)
            f[k] -= sub[k];

        /* f^2  (degree <= 26) */
        seq_t f2[27] = {0};
        for (idx_t i = 0; i <= 13; ++i)
            for (idx_t j = 0; j <= 13; ++j)
                f2[i + j] += f[i] * f[j];

        /* f^4  (degree <= 52) */
        for (idx_t i = 0; i <= 26; ++i)
            for (idx_t j = 0; j <= 26; ++j)
                f4[i + j] += f2[i] * f2[j];

        ready = true;
    }

    if (n < 1 || n > 52)
        return 0;          /* out of defined range */

    seq_t total = binom(52, n);
    seq_t avoid  = f4[n];
    return total - avoid;
}
