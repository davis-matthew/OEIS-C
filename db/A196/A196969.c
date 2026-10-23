/*
 * A196969  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,3,0,2,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence of order 16 a(n) = 2*a(n-1) +5*a(n-2)
 *            +2*a(n-3) -2*a(n-4) -11*a(n-5) +a(n-6) +36*a(n-7) +50*a(n-8)
 *            +20*a(n-9) -20*a(n-10) -49*a(n-11) -42*a(n-12) -20*a(n-13)
 *            +4*a(n-14) +9*a(n-15) +2*a(n-16) with initial values a(1) = 2,
 *            a(2) = 10, a(3) = 34, a(4) = 104, a(5) = 386, a(6) = 13...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    0, /* unused a(0) */
    2ULL, 10ULL, 34ULL, 104ULL, 386ULL, 1380ULL, 4800ULL,
    16794ULL, 58875ULL, 206084ULL, 721260ULL,
    2525126ULL, 8840855ULL, 30952194ULL, 108366098ULL,
    379400544ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0; /* highest computed index (>=0) */

/* Ensure the cache can hold at least 'needed' elements (indices 0..needed-1). */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* sequence is offset 1; behaviour for n<1 is undefined */

    /* Ensure capacity for index n */
    ensure_capacity((size_t)n + 1);

    /* Initialize first 16 values if not already */
    if (cache_len < 16) {
        size_t start = cache_len + 1;
        size_t end   = (n < 16) ? n : 16;
        for (size_t i = start; i <= end; ++i)
            cache[i] = init_vals[i];
        cache_len = end;
    }

    if (n <= cache_len)
        return cache[n];

    /* Compute missing terms using recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;

        /* Positive coefficient terms */
        sum += (seq_t)2  * cache[i-1];
        sum += (seq_t)5  * cache[i-2];
        sum += (seq_t)2  * cache[i-3];
        sum +=          cache[i-6];
        sum += (seq_t)36 * cache[i-7];
        sum += (seq_t)50 * cache[i-8];
        sum += (seq_t)20 * cache[i-9];
        sum += (seq_t)4  * cache[i-14];
        sum += (seq_t)9  * cache[i-15];
        sum += (seq_t)2  * cache[i-16];

        /* Negative coefficient terms (subtracted) */
        sum -= (seq_t)2  * cache[i-4];
        sum -= (seq_t)11 * cache[i-5];
        sum -= (seq_t)20 * cache[i-10];
        sum -= (seq_t)49 * cache[i-11];
        sum -= (seq_t)42 * cache[i-12];
        sum -= (seq_t)20 * cache[i-13];

        cache[i] = sum;
    }

    cache_len = (size_t)n;
    return cache[n];
}
