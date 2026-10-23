/*
 * A128737  Number of LDU's in all skew Dyck paths of semilength n.
 * https://oeis.org/A128737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((73*n^2-273*n+140)*a(n-1) + (-132*n^2+641*n-734)*a(n-2)
 *            + (n-3)*(89*n-269)*a(n-3) -20*(n-3)*(n-4)*a(n-4)) /
 *            (10*(n+1)*(n-4)) for n ≥ 5; a(0)=a(1)=a(2)=a(3)=0, a(4)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored (size of cache) */

    /* initialise base cases once */
    if (!cache) {
        const idx_t base_n = 5;        /* we store indices 0..4 */
        cache = (seq_t *)malloc(base_n * sizeof(seq_t));
        if (!cache) return 0;         /* allocation failed */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache_size = base_n;
    }

    /* if requested index already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;                 /* allocation failed */
    cache = tmp;

    for (idx_t k = cache_size; k <= n; ++k) {
        if (k < 5) {                    /* base values already set */
            continue;
        }

        /* use int64_t for coefficients to avoid overflow in 64‑bit */
        int64_t kk = (int64_t)k;

        int64_t coeff1      = 73 * kk * kk - 273 * kk + 140;          /* positive */
        int64_t coeff_neg2  = 132 * kk * kk - 641 * kk + 734;        /* magnitude of negative term */
        int64_t coeff3      = (kk - 3) * (89 * kk - 269);            /* positive */
        int64_t coeff_neg4  = 20 * (kk - 3) * (kk - 4);              /* magnitude of negative term */

        /* convert coeffs to seq_t (via implicit cast) */
        seq_t c1   = (seq_t)coeff1;
        seq_t c2   = (seq_t)coeff_neg2;
        seq_t c3   = (seq_t)coeff3;
        seq_t c4   = (seq_t)coeff_neg4;

        /* compute each contribution */
        seq_t term1 = cache[k-1] * c1;
        seq_t term3 = cache[k-3] * c3;
        seq_t term2 = cache[k-2] * c2;   /* will be subtracted */
        seq_t term4 = cache[k-4] * c4;   /* will be subtracted */

        /* numerator = term1 + term3 - term2 - term4  (always non‑negative) */
        seq_t numerator = term1 + term3;
        numerator -= term2;
        numerator -= term4;

        /* denominator = 10*(k+1)*(k-4)  (fits in 64‑bit for the intended n range) */
        seq_t denom = (seq_t)(10 * (kk + 1) * (kk - 4));

        cache[k] = numerator / denom;
    }

    cache_size = n + 1;
    return cache[n];
}
