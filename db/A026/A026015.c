/*
 * A026015  a(n) = number of (s(0), s(1), ..., s(2n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| = 1 for i = 1,2,...,n,
 *          s(0) = 2, s(2n) = 8. Also a(n) = T(2n,n-3), where T is the array
 *          defined in A026009.
 * https://oeis.org/A026015
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n, n-3) - binomial(2n, n-6) (OEIS A026015)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;         /* numerator factor                */
        idx_t den = i;                 /* denominator factor              */

        /* cancel common factors with current result */
        seq_t g1 = gcd(result, (seq_t)den);
        if (g1 != 1) {
            result /= g1;
            den    /= (idx_t)g1;
        }

        /* cancel any remaining common factors between
           the new numerator and denominator             */
        seq_t g2 = gcd((seq_t)num, (seq_t)den);
        if (g2 != 1) {
            num    /= (idx_t)g2;
            den    /= (idx_t)g2;
        }

        /* after cancelation den should be 1; multiply */
        result = result * (seq_t)num;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* undefined below offset */

    idx_t two_n = n << 1;               /* 2*n */
    seq_t term1 = binom_local(two_n, n - 3);
    seq_t term2 = 0;
    if (n >= 6)                         /* binom(2n, n-6) valid only for n≥6 */
        term2 = binom_local(two_n, n - 6);

    return term1 - term2;
}
