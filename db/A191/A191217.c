/*
 * A191217  Numbers k such that sigma(k) is congruent to 2 modulo 4.
 * https://oeis.org/A191217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 9433, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191217: a(n) is the n-th positive integer k such that
   sigma(k) is congruent to 2 modulo 4. */
#include <stdlib.h>
#include "../utils/sigma_of.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* cache of already found terms */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
#endif
    static idx_t checked_upto = 0;   /* last positive integer examined */

    /* Extend cache until it holds at least n terms */
    while (
#ifdef __cplusplus
           cache.size() < n
#else
           cache_len < n
#endif
          )
    {
        ++checked_upto;                         /* next candidate */
        seq_t s = sigma_of(checked_upto);        /* σ(k) */
        if ((s & 3) == 2) {                     /* σ(k) ≡ 2 (mod 4) */
#ifdef __cplusplus
            cache.push_back((seq_t)checked_upto);
#else
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len] = (seq_t)checked_upto;
#endif
#ifdef __cplusplus
            /* vector automatically updates its size */
#else
            ++cache_len;
#endif
        }
    }

    /* Return the requested element (1‑based index) */
    return
#ifdef __cplusplus
        cache[n - 1];
#else
        cache[n - 1];
#endif
}
