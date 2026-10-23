/*
 * A128725  Number of skew Dyck paths of semilength n having no LL's.
 * https://oeis.org/A128725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence from OEIS entry a(n) = (30*n*(5*n-4)*a(n-1) -
 *            6*(4*n-5)*(6*n-11)*a(n-2) - 6*(2*n*n-49*n+117)*a(n-3) -
 *            4*(19*n*n-110*n+141)*a(n-4) - 24*(n-5)*(n-6)*a(n-5) +
 *            16*(n-5)*(n-6)*a(n-6)) / (25*n*(n+1)) Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=9, a(4)=30, a(5)=107, a(6)=399
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial terms */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)9,
        (seq_t)30, (seq_t)107, (seq_t)399
    };

    /* initialise cache if first call */
    if (cache_size == 0) {
        cache_size = 7;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
    }

    /* extend cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* memory exhausted */
        cache = new_cache;

        /* compute missing terms */
        for (idx_t k = cache_size; k <= n; ++k) {
            if (k <= 6) {
                cache[k] = init[k];
                continue;
            }

            idx_t m = k;
            seq_t a_nm1 = cache[m - 1];
            seq_t a_nm2 = cache[m - 2];
            seq_t a_nm3 = cache[m - 3];
            seq_t a_nm4 = cache[m - 4];
            seq_t a_nm5 = cache[m - 5];
            seq_t a_nm6 = cache[m - 6];

            /* numerator terms (arithmetic may be signed when seq_t is signed) */
            seq_t num = 0;
            seq_t t1 = (seq_t)30 * (seq_t)m * ((seq_t)5 * m - (seq_t)4) * a_nm1;
            num += t1;
            seq_t t2 = (seq_t)(-6) * ((seq_t)4 * m - (seq_t)5) *
                       ((seq_t)6 * m - (seq_t)11) * a_nm2;
            num += t2;
            seq_t t3 = (seq_t)(-6) * ((seq_t)2 * m * m - (seq_t)49 * m + (seq_t)117) *
                       a_nm3;
            num += t3;
            seq_t t4 = (seq_t)(-4) * ((seq_t)19 * m * m - (seq_t)110 * m + (seq_t)141) *
                       a_nm4;
            num += t4;
            seq_t t5 = (seq_t)(-24) * ((seq_t)m - (seq_t)5) *
                       ((seq_t)m - (seq_t)6) * a_nm5;
            num += t5;
            seq_t t6 = (seq_t)16 * ((seq_t)m - (seq_t)5) *
                       ((seq_t)m - (seq_t)6) * a_nm6;
            num += t6;

            seq_t den = (seq_t)25 * (seq_t)m * ((seq_t)m + (seq_t)1);
            cache[m] = num / den;
        }
        cache_size = new_size;
    }

    return cache[n];
}
