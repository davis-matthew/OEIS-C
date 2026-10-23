/*
 * A180234  Demi-tribonacci numbers (rounding down): a(0)=a(1)=0, a(2)=2; a(n)
 *          = floor( (a(n-1)+a(n-2)+a(n-3))/2 ).
 * https://oeis.org/A180234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=2; a(n) = floor((a(n‑1)+a(n‑2)+a(n‑3))/2)
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to store computed terms */
static seq_t *cache = NULL;   /* array of seq_t values */
static idx_t cache_len = 0;   /* next free index (size of cache) */

/* Ensure that cache[0..n] are defined. */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        /* initialise base values */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = 0;                 /* a(0) */
        cache[1] = 0;                 /* a(1) */
        cache[2] = 2;                 /* a(2) */
        cache_len = 3;
    }
    if (cache_len > n) return;        /* already computed */

    /* grow the array to hold indices up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = (cache[i - 1] + cache[i - 2] + cache[i - 3]) >> 1;
        cache[i] = v;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 2;
    ensure_cache(n);
    return cache[n];
}
