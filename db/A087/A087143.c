/*
 * A087143  Numbers n such that sum of digits of n is divisible by digital
 *          root of n.
 * https://oeis.org/A087143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A087143 – Numbers n such that the sum of digits of n is divisible
 *  by the digital root of n.  The function performs a simple brute‑force
 *  search, caching all found terms up to the largest index requested.
 *  It efficiently reuses the cache if subsequent calls ask for larger
 *  indices. */

#include <stdlib.h>
#include <stdbool.h>

static bool term_ok(seq_t n)
{
    /* Compute the sum of digits of n. */
    seq_t s = 0;
    seq_t t = n;
    while (t > 0) {
        s += t % 10;
        t /= 10;
    }
    /* Digital root: 1..9, with 9 for multiples of 9. */
    seq_t dr = s % 9;
    if (dr == 0) dr = 9;
    return (s % dr) == 0;
}

static seq_t *cache = NULL;          /* Cached values a(1), a(2), … */
static size_t  cache_capacity = 0;   /* Allocated size of cache array   */
static size_t  cache_size     = 0;   /* Number of values currently stored */
static seq_t   last_checked   = 0;   /* Last integer that was examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but avoid overflow */

    /* If we already have this term, return it immediately. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Extend the cache until it contains at least n terms. */
    seq_t cur = last_checked + 1;
    while (cache_size < n) {
        if (term_ok(cur)) {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_capacity) {
                size_t new_cap = (cache_capacity == 0) ? 64 : cache_capacity * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1); /* allocation failed */
                cache = new_cache;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = cur;
            last_checked = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
