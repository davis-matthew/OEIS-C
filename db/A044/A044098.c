/*
 * A044098  Numbers k such that the string 4,1 occurs in the base 5
 *          representation of k but not of k-1.
 * https://oeis.org/A044098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th element of OEIS A044098:  
   numbers k such that the base‑5 representation of k contains the substring
   "41" while the representation of k‑1 does not.  
   The first term is a(1)=21.  The implementation searches for the
   required numbers and caches the results so that repeated calls are fast. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static bool has41(seq_t x)
{
    /* Detect “41” in the base‑5 digits of x (least significant first).*/
    seq_t prev = x % 5;
    x /= 5;
    while (x != 0) {
        seq_t cur = x % 5;
        if (cur == 4 && prev == 1)
            return true;
        prev = cur;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation: use std::vector for dynamic array. */
    static std::vector<seq_t> cache;
    static seq_t next_k = 1;

    while (cache.size() < (size_t)n) {
        seq_t candidate = next_k;
        while (!(has41(candidate) && !has41(candidate - 1)))
            ++candidate;
        cache.push_back(candidate);
        next_k = candidate + 1;
    }
    return cache[(size_t)(n - 1)];
#else
    /* C implementation: use a reallocable array of seq_t. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;
    static seq_t next_k = 1;

    if (cache == NULL) {
        cache_cap = 64;                           /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Assume allocation succeeds. */
    }

    while (cache_used < (size_t)n) {
        seq_t candidate = next_k;
        while (!(has41(candidate) && !has41(candidate - 1)))
            ++candidate;
        if (cache_used >= cache_cap) {
            /* Grow capacity: double it. */
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* Assume reallocation succeeds. */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_used++] = candidate;
        next_k = candidate + 1;
    }
    return cache[(size_t)(n - 1)];
#endif
}
