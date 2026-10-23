/*
 * A161475  Number of reduced words of length n in the Weyl group A_14.
 * https://oeis.org/A161475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in Product_{k=1..14} (1 -
 *            x^(k+1))/(1-x)^14
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms stored           */
    static const size_t CACHE_CAP = 16384; /* sufficient for n up to ~10 000    */

    if (!cache) {
        cache = alloc_seq_array(CACHE_CAP);
        cache_len = 0;
    }

    /* Binomial coefficients C(14,i) */
    static const seq_t C[15] = {
        1, 14, 91, 364, 1001, 2002, 3003, 3432,
        3003, 2002, 1001, 364, 91, 14, 1
    };

    /* Coefficients of the numerator polynomial P(x)=∏_{k=1}^{14}(1-x^{k+1})  */
    static int P[120];
    static int P_computed = 0;
    if (!P_computed) {
        int poly[120] = {0};
        poly[0] = 1;
        int cur_len = 1;
        for (int k = 1; k <= 14; ++k) {
            int shift = k + 1;                /* exponent of x in the factor */
            /* multiply in place: poly(x) *= (1 - x^shift) */
            for (int i = cur_len - 1; i >= 0; --i)
                poly[i + shift] -= poly[i];
            cur_len += shift;
        }
        for (int i = 0; i < 120; ++i)
            P[i] = poly[i];
        P_computed = 1;
    }

    /* Compute missing terms up to index n */
    while (cache_len <= (size_t)n) {
        size_t idx = cache_len;          /* current index to compute     */
        size_t upto = (idx < 14) ? idx : 14;

        seq_t sum_even = 0;
        seq_t sum_odd  = 0;

        /* sum_{i=1..upto} C[i] * a[idx-i] with sign handling */
        for (size_t i = 1; i <= upto; ++i) {
            seq_t prod = C[i] * cache[idx - i];
            if ((i & 1) == 0)       /* even i -> sign +1 in S */
                sum_even += prod;
            else                    /* odd  i -> sign -1 in S */
                sum_odd  += prod;
        }

        /* temp = sum_odd + p[idx]   (p[idx] may be negative) */
        seq_t temp = sum_odd;
        int pcoeff = (idx < 120) ? P[idx] : 0;
        if (pcoeff >= 0)
            temp += (seq_t)pcoeff;
        else
            temp -= (seq_t)(-pcoeff);

        /* a[idx] = temp - sum_even */
        seq_t val = temp - sum_even;
        cache[cache_len++] = val;
    }

    return cache[n];
}
