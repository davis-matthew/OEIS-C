/*
 * A037606  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3.
 * https://oeis.org/A037606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1, a(n) = a(n-1)*6 + ((n-1)%3)+1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is the decimal interpretation of the first n digits of
 * the periodic base‑6 digit sequence 1,2,3,1,2,3,...
 * This implementation memoises the results in a static cache.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* stored values a(1)..a(cache_len) */
static idx_t   cache_len = 0;        /* number of values cached */
static idx_t   cache_cap = 0;        /* capacity of the cache allocation */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? (size_t)cache_cap * 2ULL : 1ULL;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but safety */

    /* Initialise cache with a(1)=1 if needed. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_len = 1;
        cache_cap = 1;
    }

    /* Extend cache up to n. */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* i is 1‑based index; cache array is 0‑based. */
            seq_t prev  = cache[i - 2];                     /* a(i‑1)   */
            seq_t digit = (seq_t)(((i - 1) % 3) + 1);        /* 1,2,3 cycle */
            cache[i - 1] = prev * (seq_t)6 + digit;        /* a(i)     */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
