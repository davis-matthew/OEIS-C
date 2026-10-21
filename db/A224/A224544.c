/*
 * A224544  Number of (n+1) X 3 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)-5*a(n-2)+a(n-3)+2*a(n-4)-a(n-5), for n ≥ 6, with
 *            a(1)=16, a(2)=32, a(3)=52, a(4)=86, a(5)=137
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* 1‑based cache; cache[n] = a(n) */
static idx_t cache_size = 0;         /* largest n for which cache[n] is valid */

/* Ensure cache holds values up to n.  Uses linear recurrence. */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;
    if (cache_size >= n) return;

    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) abort();   /* allocation failure – should not happen in tests */
    cache = new_cache;

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)            cache[i] = (seq_t)16;
        else if (i == 2)       cache[i] = (seq_t)32;
        else if (i == 3)       cache[i] = (seq_t)52;
        else if (i == 4)       cache[i] = (seq_t)86;
        else if (i == 5)       cache[i] = (seq_t)137;
        else {
            seq_t t = 4 * cache[i-1];
            t -= 5 * cache[i-2];
            t +=   cache[i-3];
            t += 2 * cache[i-4];
            t -=   cache[i-5];
            cache[i] = t;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
