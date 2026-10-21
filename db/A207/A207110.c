/*
 * A207110  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 46, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207110: a(n) = 8*a(n-1)-28*a(n-2)+56*a(n-3)-70*a(n-4)+56*a(n-5)-28*a(n-6)+8*a(n-7)-a(n-8)
   Initial terms: a(1)=25, a(2)=625, a(3)=3399, a(4)=11511, a(5)=30415,
   a(6)=68737, a(7)=139341, a(8)=260597. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL;   /* memoisation array */
static size_t cache_cap = 0;    /* allocated capacity (number of entries) */
static size_t cache_len = 0;    /* number of terms already computed */

/* Return a(n) for the OEIS sequence A207110 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n==0, return 0 */

    /* Direct access for the first eight terms. */
    if (n <= 8) {
        static const seq_t init[8] = {
            (seq_t)25,
            (seq_t)625,
            (seq_t)3399,
            (seq_t)11511,
            (seq_t)30415,
            (seq_t)68737,
            (seq_t)139341,
            (seq_t)260597
        };
        return init[n - 1];
    }

    /* Ensure the cache can hold at least n entries. */
    if (!cache || cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        /* cast because realloc returns void* in C and requires a cast in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first eight terms if this is the first run. */
    if (cache_len == 0) {
        cache[0] = (seq_t)25;
        cache[1] = (seq_t)625;
        cache[2] = (seq_t)3399;
        cache[3] = (seq_t)11511;
        cache[4] = (seq_t)30415;
        cache[5] = (seq_t)68737;
        cache[6] = (seq_t)139341;
        cache[7] = (seq_t)260597;
        cache_len = 8;
    }

    /* Compute any missing terms up to n. */
    while (cache_len < n) {
        idx_t i = (idx_t)cache_len;   /* next index (0‑based) */

        seq_t pos = cache[i - 1] * (seq_t)8
                  + cache[i - 3] * (seq_t)56
                  + cache[i - 5] * (seq_t)56
                  + cache[i - 7] * (seq_t)8;

        seq_t neg = cache[i - 2] * (seq_t)28
                  + cache[i - 4] * (seq_t)70
                  + cache[i - 6] * (seq_t)28
                  + cache[i - 8];

        cache[cache_len++] = pos - neg;
    }

    return cache[n - 1];
}
