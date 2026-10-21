/*
 * A099491  A Chebyshev transform of Padovan numbers.
 * https://oeis.org/A099491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-2)+a(n-3)-2*a(n-4)-a(n-6), with initial values
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=-1, a(5)=-2
 * Range:     i8 n <= 20, i16 n <= 40, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;  // number of terms stored
static idx_t cache_cap   = 0;  // allocated capacity

static void init_cache(void)
{
    /* allocate initial capacity */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return; /* allocation failure, caller will get 0 */

    /* base terms */
    cache[0] = 1;
    cache[1] = 0;
    cache[2] = 0;
    cache[3] = 1;
    cache[4] = -1;
    cache[5] = -2;

    cache_size = 6;  /* we have terms 0..5 stored */
}

static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    while (cache_cap <= n) cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

static void compute_terms_up_to(idx_t n)
{
    if (cache_size > n) return; /* already computed */
    if (!cache) init_cache();
    /* compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        ensure_capacity(i);
        seq_t val = (-2)*cache[i-2] + cache[i-3] - 2*cache[i-4] - cache[i-6];
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (!cache) init_cache();
    compute_terms_up_to(n);
    return cache[n];
}
