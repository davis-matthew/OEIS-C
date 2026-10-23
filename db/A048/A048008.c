/*
 * A048008  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(floor((n-1)/3),
 *            floor((n-floor((n-1)/3))/3))} binom(floor((n-1)/3),
 *            k)*binom(n-floor((n-1)/3), 3*k)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t L = (n - 1) / 3;
    idx_t kmax1 = L;
    idx_t kmax2 = (n - L) / 3;
    idx_t kmax = kmax1 < kmax2 ? kmax1 : kmax2;
    if (kmax < 1) return 0;

    seq_t sum = 0;
    seq_t binomL = 1;                       /* binom(L, 0) */

    /* Precompute binom(N, j) for j up to 3*kmax where N = n-L */
    idx_t N = n - L;
    size_t lenN = (size_t)(3 * kmax);
    seq_t *bn = (seq_t *)malloc((lenN + 1) * sizeof(seq_t));
    if (bn == NULL) return 0;              /* allocation failed */
    bn[0] = 1;
    for (idx_t j = 0; j < lenN; ++j) {
        idx_t next = j + 1;
        bn[next] = bn[j] * (N - j) / next;
    }

    for (idx_t k = 1; k <= kmax; ++k) {
        binomL = binomL * (L - k + 1) / k;
        seq_t term = binomL * bn[3 * k];
        sum += term;
    }

    free(bn);
    return sum;
}
