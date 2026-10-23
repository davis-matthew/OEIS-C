/*
 * A179531  a(0)= 0, a(1)= 1; for n>1, a(n) = a(n-2) - min{a(n-1),n} if that
 *          number is positive, otherwise a(n) = a(n-2) + max{a(n-1),n}.
 * https://oeis.org/A179531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; for n>1, a(n) = a(n-2) - min{a(n-1), n} if
 *            that value is positive, otherwise a(n) = a(n-2) + max{a(n-1), n}
 * Range:     u8 n <= 112, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* internal static cache */
    static seq_t *cache     = NULL;   /* pointer to cached terms     */
    static size_t cache_cap = 0;      /* allocated capacity (in terms) */
    static size_t cache_len = 0;      /* number of terms already computed (highest index + 1) */

    /* ----------  initialise cache on first call  ---------- */
    if (!cache) {
        size_t init_len = 2;                          /* we always need indices 0 and 1 */
        if ((size_t)(n + 1) > init_len)               /* but grow if n is very small in later calls */
            init_len = (size_t)(n + 1);

        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) abort();

        cache_cap = init_len;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* ----------  grow cache if needed  ---------- */
    size_t needed_cap = (size_t)(n + 1);
    if (needed_cap > cache_cap) {
        while (cache_cap < needed_cap)
            cache_cap = cache_cap * 2 ? : 2;   /* double capacity */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* ----------  compute terms up to n if not already cached  ---------- */
    if (n + 1 > cache_len) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t a_nm2 = cache[i - 2];
            seq_t a_nm1 = cache[i - 1];
            seq_t n_seq = (seq_t)i;

            seq_t m = (a_nm1 < n_seq) ? a_nm1 : n_seq;   /* min{a(n-1), n} */
            seq_t M = (a_nm1 > n_seq) ? a_nm1 : n_seq;   /* max{a(n-1), n} */

            if (a_nm2 > m)                  /* a(n-2) - m is positive */
                cache[i] = a_nm2 - m;
            else
                cache[i] = a_nm2 + M;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
