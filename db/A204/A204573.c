/*
 * A204573  A204519(n)^2 = floor(A055851(n)/6): Squares which written in base
 *          6, with some digit appended, yield another square.
 * https://oeis.org/A204573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 99*a(n-3) - 99*a(n-6) + a(n-9) for n>10; initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=4, a(6)=16, a(7)=121,
 *            a(8)=400, a(9)=1600, a(10)=11881
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initial[10] = {
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)4,
    (seq_t)16, (seq_t)121, (seq_t)400, (seq_t)1600, (seq_t)11881
};

static seq_t *cache = NULL;   /* dynamic cache of computed values           */
static idx_t cache_len = 0;   /* number of values currently stored (last index) */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Ensure the cache can hold at least 'needed' elements (indices 1..needed). */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Returns the n-th term of A204573 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset – safe result   */

    if (n <= 10) return initial[n - 1];   /* direct return for the known 1…10   */

    /* initialise cache on first use */
    if (cache == NULL) {
        ensure_capacity(10);
        for (idx_t i = 0; i < 10; ++i) cache[i] = initial[i];
        cache_len = 10;
    }

    /* grow cache and compute all missing terms up to n */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t a3 = cache[(i-3)-1];   /* a(i-3) */
            seq_t a6 = cache[(i-6)-1];   /* a(i-6) */
            seq_t a9 = cache[(i-9)-1];   /* a(i-9) */
            cache[i-1] = (seq_t)99 * a3 - (seq_t)99 * a6 + a9;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
