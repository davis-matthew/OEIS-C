/*
 * A177171  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=0 and l=-2.
 * https://oeis.org/A177171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(0)=1, a(1)=9,
 * a(n+1) = ∑_{p=0}^{n} a(p) * a(n-p) - 2   for n ≥ 1
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t cache_len = 0;          /* number of terms stored (next free index) */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    /* initial allocation */
    if (!cache) {
        cache_cap = 2;                   /* start with space for a(0), a(1) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = 1;                /* a(0) */
        } else if (cache_len == 1) {
            cache[1] = 9;                /* a(1) */
        } else {
            idx_t m = cache_len - 1;     /* n in recurrence a(n+1) = ... */
            seq_t sum = 0;
            for (idx_t p = 0; p <= m; ++p)
                sum += cache[p] * cache[m - p];
            sum -= 2;                    /* subtract 2 */

            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = sum;
        }
        ++cache_len;
    }
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
