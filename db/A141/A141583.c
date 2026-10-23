/*
 * A141583  Squares of tribonacci numbers A000213.
 * https://oeis.org/A141583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A000213(n))^2 Recurrence: a(n) = 2*a(n-1) + 3*a(n-2) +
 *            6*a(n-3) - a(n-4) - a(n-6) Initial values: a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=9, a(4)=25, a(5)=81
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL; /* array of computed terms */
static size_t cache_cap = 0;   /* current allocated capacity */
static idx_t cache_last = (idx_t)(-1); /* last index computed */

/* Ensure the cache can hold terms up to 'n'.  The array is grown lazily. */
static void ensure_cache(idx_t n)
{
    /* Allocate and initialise the cache on first use. */
    if (!cache) {
        cache_cap = n + 1 < 16 ? 16 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure – caller will see garbage */

        /* Initialise the first six tribonacci‑squared terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)81;
        cache_last = 5;
    }

    /* Grow the cache if the requested index exceeds the current capacity. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – ignore */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to index n. */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* For i < 6 the value is already set by the initialisation. */
            if (i < 6) continue;
            seq_t t = 2 * cache[i - 1];
            t += 3 * cache[i - 2];
            t += 6 * cache[i - 3];
            t -= cache[i - 4];
            t -= cache[i - 6];
            cache[i] = t;
        }
        cache_last = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
