/*
 * A120400  Expansion of 1/(1-x-x^2-x^6).
 * https://oeis.org/A120400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-6) with a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, a(5)=8
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 43, u64 n <= 87, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Ensure cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* Grow if necessary */
    if ((n + 1) > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        /* Must store at least the first 6 entries */
        if (new_cap < 6) new_cap = 6;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First time: initialise the known base values */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)8;
        cache_len = 6;            /* values 0..5 are ready */
    }

    /* Compute any remaining values up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* i-6 is safe because i>=cache_len>=6 */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-6];
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
