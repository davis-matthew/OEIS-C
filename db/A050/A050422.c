/*
 * A050422  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 3.
 * https://oeis.org/A050422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   satisfies_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Property: minimal number of digit deletions needed to leave a binary
    palindrome (allowing leading zeroes) is exactly 3.
    The function computes a(n) by brute‑forcing increasing numbers, caching
    the ones that satisfy the property.  DP of size 64×64 is used to
    compute the longest palindromic subsequence – O(1) per candidate.  */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include <string.h>   /* memset */
#include "../utils/satisfies_property.h"

static seq_t *cache = NULL;
static size_t cache_used  = 0;   /* number of elements stored */
static size_t cache_cap   = 0;   /* allocated capacity */
static seq_t next_candidate = 1; /* current integer to test */

/* enlarge the cache when necessary */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t newsize = new_cap * sizeof(seq_t);
    seq_t *tmp;
    if (cache == NULL)
        tmp = (seq_t *)malloc(newsize);
    else
        tmp = (seq_t *)realloc(cache, newsize);
    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence routine */
seq_t a(idx_t n)
{
    /* grow the cache as long as needed */
    while (cache_used < n) {
        if (satisfies_property(next_candidate)) {
            if (cache_used + 1 > cache_cap)
                ensure_cache_capacity((cache_cap == 0) ? 16 : cache_cap * 2);
            cache[cache_used++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
