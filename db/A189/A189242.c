/*
 * A189242  Numbers n such that 24*n+17 is not prime.
 * https://oeis.org/A189242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189242: Numbers n such that 24*n+17 is composite (search). */
#include <stdlib.h>
#include <stddef.h>
#if !defined(__cplusplus)
# include <stdbool.h>
#endif
#include "../utils/is_prime.h"

/*--- Static cache for previously computed terms ---------------------*/
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;      /* stores a(1), a(2), …          */
static idx_t last_m = 0;              /* last tested m (C++ build)    */
#else
static seq_t *cache = NULL;          /* stores a(1), a(2), …          */
static size_t cache_cap = 0;          /* allocated capacity of cache   */
static size_t cache_len = 0;          /* number of terms stored        */
static idx_t last_m = 0;              /* last tested m (C build)       */
#endif

/* Resize cache to at least 'needed' entries. */
static void grow_cache(size_t needed)
{
#ifdef __cplusplus
    /* std::vector grows automatically. */
    (void)needed;
#else
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/*--- Helper: check compositeness of 24*m+17 ------------------------*/
static bool is_composite_for_m(idx_t m)
{
    idx_t test = 24 * m + 17;          /* value to test for primality   */
    return !is_prime(test);
}

/*--- Main function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, guard against overflow */

    while (true) {
#ifdef __cplusplus
        if (cache.size() >= n) break;
#else
        if (cache_len >= n) break;
#endif
        /* Check next integer m = last_m + 1. */
        idx_t m = last_m + 1;
        if (is_composite_for_m(m)) {
#ifdef __cplusplus
            cache.push_back((seq_t)m);
#else
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
#endif
        }
        last_m = m;
    }

#ifdef __cplusplus
    return cache.at(n - 1);
#else
    return cache[n - 1];
#endif
}
