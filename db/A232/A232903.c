/*
 * A232903  Number of (n+1)X(3+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally.
 * https://oeis.org/A232903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) + 79*a(n-2) + 128*a(n-3)
 *                  - 788*a(n-4) + 196*a(n-5)
 *                  - 644*a(n-6) + 416*a(n-7)
 *                  - 64*a(n-8)
 *
 * Initial values (n = 1 .. 8):
 *   a(1)=52, a(2)=320, a(3)=4928, a(4)=62276,
 *   a(5)=761860, a(6)=9880580, a(7)=123587140, a(8)=1571314692
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;   /* array of seq_t, index 0 is unused */
static idx_t   cache_last = 0;/* highest n that is computed and stored */
static size_t  cache_cap  = 0;/* allocated capacity of cache (in elements) */

/* ensure the cache has at least (n+1) elements; resize if necessary */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* value outside the definition – return 0 */

    ensure_cache(n);

    /* initialise the first eight values if they are not yet cached */
    if (cache_last < 8) {
        static const uint64_t init64[8] = {
            52ULL, 320ULL, 4928ULL, 62276ULL,
            761860ULL, 9880580ULL, 123587140ULL, 1571314692ULL
        };
        idx_t start = cache_last + 1;
        idx_t end   = (n < 8) ? n : 8;
        for (idx_t i = start; i <= end; ++i)
            cache[i] = (seq_t)init64[i - 1];
        cache_last = end;
    }

    /* return already computed value */
    if (n <= cache_last)
        return cache[n];

    /* compute terms from cache_last+1 up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t pos = (seq_t)6 * cache[i - 1]
                  + (seq_t)79 * cache[i - 2]
                  + (seq_t)128 * cache[i - 3];
        pos += (seq_t)196 * cache[i - 5]
                 + (seq_t)416 * cache[i - 7];

        seq_t neg = (seq_t)788 * cache[i - 4]
                    + (seq_t)644 * cache[i - 6]
                    + (seq_t)64 * cache[i - 8];

        cache[i] = pos - neg;   /* pos > neg for all n ≥ 9, so no underflow in the intended domain */
    }
    cache_last = n;
    return cache[n];
}
