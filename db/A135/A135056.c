/*
 * A135056  Pentanacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5) if n>=5, and a(n) = n otherwise.
 * https://oeis.org/A135056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n (0 ≤ n < 5) a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)
 *            (n ≥ 5)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array storing computed terms */
static idx_t cache_len = 0;      /* number of elements stored in cache */

/* Extend the cache so that it contains values up to and including n */
static void compute_to(idx_t n)
{
    if (cache_len > n)
        return;                    /* already computed */

    idx_t old_len = cache_len;      /* number of terms currently stored */
    idx_t new_len = n + 1;          /* we need indices 0..n inclusive */

    /* Allocate or grow the cache */
    if (old_len == 0) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) abort();        /* allocation failed */
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
    }
    cache_len = new_len;

    /* Compute all new terms, reusing existing values when possible */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i < 5) {
            cache[i] = (seq_t)i;
        } else {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
        }
    }
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
