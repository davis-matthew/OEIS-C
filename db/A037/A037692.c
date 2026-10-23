/*
 * A037692  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0,3.
 * https://oeis.org/A037692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(n)=a(n-1)*8 + d_n where d_n = [1,2,0,3][(n-1)%4] */

#include <stdlib.h>
#include <stdint.h>

/* Cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* Number of elements allocated */
static size_t cache_size = 0;  /* Highest index computed (1‑based) */

/* Periodic digit array */
static const uint8_t period[4] = {1, 2, 0, 3};

/* Resize cache to hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n<1, but return 0 to avoid UB. */

    size_t req = (size_t)n;

    /* Ensure storage. */
    ensure_cache_capacity(req);

    /* Compute missing terms. */
    if (cache_size < req) {
        for (size_t k = cache_size; k < req; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
            } else {
                cache[k] = cache[k - 1] * (seq_t)8 + (seq_t)period[k % 4];
            }
        }
        cache_size = req;
    }

    return cache[req - 1];
}
