/*
 * A234576  Number of Weyl group elements, not containing s_1 or s_2, which
 *          contribute nonzero terms to Kostant's weight multiplicity formula
 *          when computing the multiplicity of the zero-weight in the adjoint
 *          representation for the Lie algebra of type D and rank n.
 * https://oeis.org/A234576
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) with initial values:
 *            a(4)=4, a(5)=7, a(6)=14, a(7)=34
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   make_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/make_array.h"

static void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n < 4) return 0; /* undefined for n < 4 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* array size (elements)        */
    static size_t cache_len = 0;     /* next free index (highest+1)  */

    /* initialize cache once */
    if (!cache) {
        cache_cap = 8;               /* capacity for indices 0..7 */
        cache = make_array(cache_cap);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)34;
        cache_len = 8;               /* indices 0..7 are valid now */
    }

    /* grow cache and fill missing terms up to requested index */
    if (cache_len <= (size_t)n) {
        /* ensure capacity */
        while (cache_len <= (size_t)n) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap <= cache_len) new_cap = cache_len + 1;
                seq_t *tmp = make_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
                free_array(cache);
                cache = tmp;
                cache_cap = new_cap;
            }

            /* compute next term */
            size_t i = cache_len; /* i is the index to compute (≥8) */
            seq_t term = cache[i-1] + cache[i-2] + cache[i-4];
            term += cache[i-3] * (seq_t)3;
            cache[i] = term;
            ++cache_len;
        }
    }

    return cache[(size_t)n];
}
