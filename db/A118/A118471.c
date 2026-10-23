/*
 * A118471  a(0)=1. a(n) = a(n-1)*(n+1), if n is in the sequence. a(n)= a(n-1)
 *          +1, if n is missing from the sequence.
 * https://oeis.org/A118471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118471.
   Definition:
     a(0) = 1
     for n >= 1:
       if n occurs among {a(0), a(1), ..., a(n-1)} then
           a(n) = a(n-1) * (n+1)
       else
           a(n) = a(n-1) + 1
*/

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* array of previously computed terms */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* capacity of the array */

/* increase capacity to at least new_len; abort on allocation failure */
static void grow_cache(size_t new_len) {
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < new_len) new_cap = new_len;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* binary search for value 'val' among first 'len' elements of cache */
static bool contains_value_at(idx_t val, size_t len) {
    seq_t target = (seq_t)val;
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cache[mid] == target) return true;
        if (cache[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    /* ensure the base term is present */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = 1;
        cache_len = 1;
    }

    /* extend the sequence up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t current_len = cache_len;
        bool present = contains_value_at(i, current_len);
        seq_t prev = cache[current_len - 1];
        seq_t next = present ? prev * ((seq_t)i + 1) : prev + 1;
        grow_cache(current_len + 1);
        cache[cache_len++] = next;
    }

    return cache[n];
}
