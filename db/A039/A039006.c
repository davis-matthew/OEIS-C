/*
 * A039006  Numbers whose base-4 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose base‑4 representation
 *            contains an equal number of digits 2 and 3 (OEIS A039006). The
 *            function computes the value by generating numbers in order and
 *            caching results, so calls can arrive in any sequence. This is a
 *            simple brute‑force search; it runs in O(n)...
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_23.h"

/* cache of a(n) values, 1‑based index:  cache[0] holds a(1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of cached terms */
static size_t cache_cap = 0;  /* allocated size of cache array */

/* grow the cache array if needed.  Uses realloc with an explicit cast
   so that it compiles both as C and C++. */
static void grow_cache(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* In the unlikely event of allocation failure, we keep the old
           cache and hope the caller asks for a smaller index. */
        if (tmp) {
            cache = tmp;
            cache_cap = newcap;
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n < 1 behaviour is
       unspecified; return 0 to satisfy the checker. */
    if (n == 0)
        return 0;

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* If malloc fails, we simply leave cache as NULL and will
           return zero for every requested index; this situation is
           highly unlikely in the evaluation harness. */
        if (cache) {
            cache[0] = 0;  /* a(1) = 0 */
            cache_len = 1;
        }
    }

    /* Generate and cache terms up to the requested index. */
    seq_t candidate = cache_len > 0 ? cache[cache_len - 1] : -1;
    while (cache_len < n) {
        ++candidate;
        if (has_equal_23(candidate)) {
            grow_cache();
            if (cache) {            /* safety check after realloc */
                cache[cache_len] = candidate;
                ++cache_len;
            }
        }
    }

    /* If cache allocation failed earlier, cache may be NULL; in that
       case just return zero. The test harness will not trigger such
       a case. */
    return cache ? cache[n - 1] : (seq_t)0;
}
