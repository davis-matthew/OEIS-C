/*
 * A211621  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+2x+3y>0.
 * https://oeis.org/A211621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - a(n-4) + 2*a(n-6) - a(n-7), a(0) = 0,
 *            a(1) = 3, a(2) = 29, a(3) = 103, a(4) = 247, a(5) = 484, a(6) =
 *            843. (OEIS sequence A211621) Uses a simple linear recurrence
 *            with a small cache
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t  cache_size  = 0;
static idx_t  cache_cap   = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;

    if (cache_cap <= n) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)29;
        cache[3] = (seq_t)103;
        cache[4] = (seq_t)247;
        cache[5] = (seq_t)484;
        cache[6] = (seq_t)843;
        cache_size = 7;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 7) continue;
        seq_t term = cache[i-1];
        term += cache[i-1];
        term -= cache[i-3];
        term -= cache[i-4];
        term += cache[i-6];
        term += cache[i-6];
        term -= cache[i-7];
        cache[i] = term;
    }

    if (n + 1 > cache_size) cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
