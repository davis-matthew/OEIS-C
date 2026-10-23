/*
 * A044076  Numbers k such that string 3,3 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑4 representation of
 *            k contains the consecutive digits 3,3 but the base‑4
 *            representation of k−1 does not
 * Range:     u8 n <= 15, u16 n <= 3105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#if defined(__cplusplus)
#include <vector>
#include <cstdlib>
#else
#include <stdlib.h>
#endif

/* Check whether a base‑4 representation of a number contains the pattern 3,3. */
static bool contains_33(seq_t x)
{
    seq_t prev = 0;
    bool prev_set = false;
    while (x > 0) {
        seq_t d = x % 4;
        if (prev_set && d == 3 && prev == 3)
            return true;
        prev = d;
        prev_set = true;
        x = x / 4;
    }
    return false;
}

#if defined(__cplusplus)
/* C++17 path – use std::vector for dynamic storage. */
static std::vector<seq_t> cache;
static seq_t cur_k = 0;     /* last examined number   */
static idx_t cur_found = 0; /* number of valid terms found so far */
#else
/* C11 path – minimal malloc/realloc handling. */
static seq_t *cache    = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t cur_k     = 0;
static idx_t cur_found = 0; /* number of valid terms found so far */

/* Grow the cache to at least new_cap elements. */
static void grow_cache(idx_t new_cap)
{
    seq_t *tmp = realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset – return 0 */

#if defined(__cplusplus)
    /* Grow the cache as needed. */
    while (cur_found < n) {
        cur_k = cur_k + 1;
        if (contains_33(cur_k) && !contains_33(cur_k - 1)) {
            ++cur_found;
            cache.push_back(cur_k);
        }
    }
    return cache[n - 1];
#else
    if (cache_len >= n)
        return cache[n - 1];

    while (cur_found < n) {
        cur_k = cur_k + 1;
        if (contains_33(cur_k) && !contains_33(cur_k - 1)) {
            ++cur_found;
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap * 2 : 1);
            cache[cache_len++] = cur_k;
        }
    }
    return cache[n - 1];
#endif
}
