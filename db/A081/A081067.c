/*
 * A081067  a(n) = Lucas(4n+2)+2, or 5*Fibonacci(2n+1)^2.
 * https://oeis.org/A081067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n+2)+2 = 5*Fibonacci(2n+1)^2 Recurrence: a(0)=5,
 *            a(1)=20, a(2)=125, a(n)=8*a(n-1)-8*a(n-2)+a(n-3) for n>=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values – persistent across calls */
static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_len = 0;   /* first unused index (size of valid data) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Ensure cache has room for terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already computed */

    /* Grow capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {
            /* In a real program you might handle this error; here we abort */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Fill missing entries using recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 5;
        } else if (i == 1) {
            value = 20;
        } else if (i == 2) {
            value = 125;
        } else {
            /* a(i) = 8*a(i-1) - 8*a(i-2) + a(i-3) */
            seq_t t1 = cache[i-1] * 8;
            seq_t t2 = cache[i-2] * 8;
            value = t1 - t2 + cache[i-3];
        }
        cache[i] = value;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Compute and cache terms up to n if necessary */
    ensure_cache(n);
    return cache[n];
}
