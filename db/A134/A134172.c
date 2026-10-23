/*
 * A134172  Expansion of x^2*(1+x)*(1-x+x^2) / ((1-x)^2*(1+x^2)^2).
 * https://oeis.org/A134172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)-3*a(n-2)+4*a(n-3)-3*a(n-4)+2*a(n-5)-a(n-6)
 *            initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=1, a(5)=1
 * Range:     u8 n <= 341, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure that values up to n have been computed and stored in cache */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;          /* cast to size_t for array indexing */
    if (cache_len > target)
        return;

    /* initialise cache storage if required */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache capacity as needed */
    while (cache_len <= target) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t i = cache_len;
        seq_t value;

        /* initial values */
        if (i == 0)          value = 0;
        else if (i == 1)     value = 0;
        else if (i == 2)     value = 1;
        else if (i == 3)     value = 2;
        else if (i == 4)     value = 1;
        else if (i == 5)     value = 1;
        else {
            /* compute via recurrence, using signed long long for safety */
            long long v = 0;
            v += 2LL * (long long)cache[i-1];
            v -= 3LL * (long long)cache[i-2];
            v += 4LL * (long long)cache[i-3];
            v -= 3LL * (long long)cache[i-4];
            v += 2LL * (long long)cache[i-5];
            v -= (long long)cache[i-6];
            value = (seq_t)v;
        }

        cache[i] = value;
        cache_len++;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
