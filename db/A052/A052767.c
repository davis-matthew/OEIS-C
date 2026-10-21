/*
 * A052767  Expansion of e.g.f.: -(log(1-x))^5.
 * https://oeis.org/A052767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 120 * Stirling1(n,5) (unsigned Stirling numbers of the
 *            first kind)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    /* Previous Stirling numbers S(k,0..5) for the last computed n */
    static seq_t prev_c[6];
    static idx_t prev_n = 0;
    static int initialized = 0;

    /* --- one‑time initialization -------------------------------------- */
    if (!initialized) {
        /* S(0,0)=1, S(0,k>0)=0 */
        prev_c[0] = (seq_t)1;
        for (int k = 1; k < 6; ++k) prev_c[k] = (seq_t)0;

        /* cache[0] holds a(0) = 0 */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed – unlikely */
        cache[0] = (seq_t)0;
        cache_size = 1;
        prev_n = 0;
        initialized = 1;
    }

    /* --- grow cache if necessary ------------------------------------- */
    if (n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failed */
        cache = new_cache;
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;              /* initialise new slots */
        cache_size = new_size;
    }

    /* --- compute all terms up to n if we haven't yet ----------------- */
    for (idx_t i = prev_n + 1; i <= n; ++i) {
        seq_t curr_c[6] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0 };

        /* S(i,0) = 0 for i > 0 */
        curr_c[0] = (seq_t)0;

        for (int k = 1; k <= 5; ++k) {
            if ((idx_t)k > i) {
                curr_c[k] = (seq_t)0;
            } else {
                seq_t term1 = prev_c[k - 1];
                seq_t term2 = (seq_t)(i - 1) * prev_c[k];
                curr_c[k] = term1 + term2;
            }
        }

        /* a(i) = 120 * S(i,5)  – for i < 5 the value is 0 */
        if (i >= 5) {
            cache[i] = (seq_t)120 * curr_c[5];
        } else {
            cache[i] = (seq_t)0;
        }

        /* shift to next state */
        for (int k = 0; k < 6; ++k)
            prev_c[k] = curr_c[k];
    }
    prev_n = n;

    return cache[n];
}
