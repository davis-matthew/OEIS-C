/*
 * A143159  Expansion of q^(-13/24) * eta(q^2) * eta(q^3) * eta(q^4)^2 in
 *          powers of q.
 * https://oeis.org/A143159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1 - x^(2k))(1 - x^(3k))(1
 *            - x^(4k))^2 (i.e. g.f. = ∏_{k>0} (1 - x^(2k)) (1 - x^(3k)) (1 -
 *            x^(4k))^2)
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate vector of coefficients 0..n, initialized to 0 */
    seq_t *coeff;
#ifdef __cplusplus
    coeff = reinterpret_cast<seq_t*>(calloc((size_t)n + 1, sizeof(seq_t)));
#else
    coeff = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
#endif
    if (!coeff)
        return 0;      /* allocation failure – unlikely for n ≤ 10000 */

    coeff[0] = 1;       /* start from the constant polynomial 1 */

    /* Apply each factor (1 - x^(2k)), (1 - x^(3k)) and (1 - x^(4k))^2
       for all positive integers k, but stop when all exponents exceed n */
    for (idx_t k = 1; ; ++k)
    {
        idx_t m2 = 2 * k;   /* exponent for (1 - x^(2k))   */
        idx_t m3 = 3 * k;   /* exponent for (1 - x^(3k))   */
        idx_t m4 = 4 * k;   /* exponent for (1 - x^(4k))   */
        bool any = false;
        if (m2 <= n) any = true;
        if (m3 <= n) any = true;
        if (m4 <= n) any = true;
        if (!any) break;   /* all remaining factors would start beyond n */

        /* Apply (1 - x^m2) */
        if (m2 <= n) {
            for (idx_t i = n; ; --i) {
                coeff[i] -= coeff[i - m2];
                if (i == m2) break;
            }
        }

        /* Apply (1 - x^m3) */
        if (m3 <= n) {
            for (idx_t i = n; ; --i) {
                coeff[i] -= coeff[i - m3];
                if (i == m3) break;
            }
        }

        /* Apply (1 - x^m4) twice */
        if (m4 <= n) {
            for (int rep = 0; rep < 2; ++rep) {
                for (idx_t i = n; ; --i) {
                    coeff[i] -= coeff[i - m4];
                    if (i == m4) break;
                }
            }
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
