/*
 * A037685  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1. For n>1, a(n) = a(n-1)*8 + d_{n-1}, where d = 1,0,3,2
 *            repeats
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates a(n) as the integer whose base‑8 digits are the first n
   terms of the periodic sequence 1,0,3,2,1,0,3,2,… */

#include <stdlib.h>   /* for malloc, realloc */

static const unsigned char digit_cycle[4] = {1, 0, 3, 2};

static seq_t *cache = NULL;       /* array holding a(1)…a(n) */
static idx_t cache_len = 0;       /* number of terms currently stored */
static idx_t cache_cap = 0;       /* capacity of the allocated array */

/* Ensure that the cache has capacity for at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_cap < n) {
        idx_t new_cap = (cache_cap == 0 ? 1 : cache_cap << 1);
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1.
       If called with n == 0, return 0. */
    if (n == 0)
        return (seq_t)0;

    ensure_cache(n);

    /* Compute missing terms up to n. */
    if (cache_len < n) {
        /* If no terms yet, start with a(1). */
        for (idx_t i = cache_len; i < n; ++i) {
            unsigned char d = digit_cycle[i % 4];
            if (i == 0) {
                cache[0] = (seq_t)d;          /* a(1) */
            } else {
                cache[i] = cache[i - 1] * (seq_t)8 + (seq_t)d;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];   /* a(n) stored at index n-1 */
}
