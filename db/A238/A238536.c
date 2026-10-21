/*
 * A238536  A fourth-order linear divisibility sequence related to the
 *          Fibonacci numbers: a(n) = (1/2)*Fibonacci(3*n)*Lucas(n).
 * https://oeis.org/A238536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+19*a(n-2)+4*a(n-3)-a(n-4) Initial values:
 *            a(1)=1, a(2)=12, a(3)=68, a(4)=504
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;    /* 1‑based indexing, index 0 unused */
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* initialise the first four values of the sequence */
static void init_cache(void)
{
    if (cache_max >= 4) return;                /* already initialised */

    ensure_cache_size(4);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)68;
    cache[4] = (seq_t)504;
    cache_max = 4;
}

/* compute the n‑th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n<1 */

    init_cache();

    if (n <= cache_max) return cache[n];

    /* compute terms up to n */
    ensure_cache_size(n);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = 4 * t1 + 19 * t2 + 4 * t3 - t4;
    }
    cache_max = n;
    return cache[n];
}
