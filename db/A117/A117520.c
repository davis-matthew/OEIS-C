/*
 * A117520  Triangular numbers for which the digital root is also a triangular
 *          number.
 * https://oeis.org/A117520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117520:  Triangular numbers whose digital root is NOT 9.
   The nth term is the nth triangular number T_m = m*(m+1)/2 for which
   T_m mod 9 ≠ 0 (i.e. digital root 1, 3, 6 or 9, but 9 is excluded).
   The progress is generated on demand and cached so that calls may
   arrive in any order. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* size_t */

seq_t a(idx_t n)
{
    /* cache of generated terms */
    static seq_t *cache   = NULL;   /* array of seq_t   */
    static size_t cache_len = 0;    /* number of terms stored   */
    static size_t cache_cap = 0;    /* allocated capacity       */
    static idx_t  next_m   = 1;     /* next triangular index     */

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 4;                /* initial capacity */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[0] = (seq_t)0;          /* T_0 = 0, the first term */
        cache_len = 1;
        next_m = 1;
    }

    /* grow the cache to be able to answer the request */
    while (cache_len < (size_t)n) {
        /* compute T_m = m*(m+1)/2 in seq_t */
        seq_t tri = (seq_t)next_m * ((seq_t)next_m + 1U) / 2U;

        /* compute T_m mod 9 without large intermediate values */
        idx_t mod9_m      = next_m % 9U;
        idx_t mod9_mnext  = (next_m + 1U) % 9U;
        idx_t prod_mod9   = (mod9_m * mod9_mnext) % 9U;      /* m*(m+1) mod 9 */
        idx_t tri_mod9    = (prod_mod9 * 5U) % 9U;            /* divide by 2 */

        /* include this triangular number unless its digital root is 9 */
        if (tri_mod9 != 0U) {
            if (cache_len == cache_cap) {
                /* double the capacity */
                size_t new_cap = (cache_cap < 8U ? 8U : cache_cap * 2U);
                seq_t *new_cache;
#ifdef __cplusplus
                new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
                delete[] cache;
#else
                new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tri;
        }
        ++next_m;
    }

    return cache[n-1];
}
