/*
 * A048019  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{min(T, U/3)} C(T,i) * C(U,3i) where T =
 *            floor((n-2)/3), U = n - T, and C(n,k) is the binomial coeff
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute T and U */
    idx_t T = (n >= 2) ? (n - 2) / 3 : 0;      /* floor((n-2)/3) */
    idx_t U = n - T;

    /* early exit: need at least one element from the first part (i>=1)
       and 3*i <= U, thus U>=3 and T>=1 */
    if (T == 0 || U < 3)
        return 0;

    /* allocate arrays for binomials */
    seq_t *cT = (seq_t *)malloc((T + 1) * sizeof(seq_t));
    seq_t *cU = (seq_t *)malloc((U + 1) * sizeof(seq_t));
    if (!cT || !cU) {
        /* Out of memory – return 0 as a safe fallback (unlikely in tests). */
        free(cT);
        free(cU);
        return 0;
    }

    /* compute binomial coefficients for T */
    cT[0] = 1;
    for (idx_t i = 1; i <= T; ++i) {
        /* cT[i] = cT[i-1] * (T - i + 1) / i */
        seq_t num = cT[i - 1] * (seq_t)(T - i + 1);
        cT[i] = num / (seq_t)i;
    }

    /* compute binomial coefficients for U */
    cU[0] = 1;
    for (idx_t i = 1; i <= U; ++i) {
        /* cU[i] = cU[i-1] * (U - i + 1) / i */
        seq_t num = cU[i - 1] * (seq_t)(U - i + 1);
        cU[i] = num / (seq_t)i;
    }

    /* sum over valid i */
    seq_t res = 0;
    idx_t max_i = T;
    if (U / 3 < max_i)
        max_i = U / 3;
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t term = cT[i] * cU[3 * i];
        res += term;
    }

    free(cT);
    free(cU);
    return res;
}
