/*
 * A178205  a(n) = a(n-1) + 10*a(n-3) for n > 2; a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A178205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-3), a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* pointer to cached terms */
static idx_t  cache_len = 0;     /* number of computed terms (size) */
static size_t cache_cap = 0;     /* current capacity of cache */

/* Increment the cache until it contains term a(n).  All required terms are
 * stored consecutively starting at index 0.  The cache is lazily created
 * and grown as needed.  The function is thread‑unsynchronized; external
 * protection is assumed. */
seq_t a(idx_t n)
{
    /* initialise cache with the three starting values */
    if (cache == NULL) {
        cache_cap = 16;                         /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = 1;
        cache_len = 3;
    }

    /* grow the cache if it does not yet contain a(n) */
    while (cache_len <= n) {
        /* enlarge the array once it is full */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;    /* safety */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* out of memory – abort the program gracefully */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute the next term in the recurrence */
        cache[cache_len] = cache[cache_len - 1] + (seq_t)10 * cache[cache_len - 3];
        ++cache_len;
    }

    return cache[n];
}
