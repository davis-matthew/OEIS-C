/*
 * A237227  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2 X 2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) + 24*a(n-4) + 8*a(n-6) for n>7 with initial
 *            values a(1)=81, a(2)=268, a(3)=928, a(4)=2784, a(5)=9628,
 *            a(6)=28912, a(7)=100008
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;    /* number of values stored */
static size_t cache_cap  = 0;    /* capacity of the cache */

/* Ensure that the cache can hold at least n elements.  */
static void ensure_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term of A237227.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined, but stay safe */

    /* Initial cache population on the first call. */
    if (cache_size == 0) {
        cache_cap = 8;            /* at least enough for the 7 base terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 81;
        cache[1] = 268;
        cache[2] = 928;
        cache[3] = 2784;
        cache[4] = 9628;
        cache[5] = 28912;
        cache[6] = 100008;
        cache_size = 7;
    }

    /* Grow the cache if needed. */
    ensure_capacity(n);

    /* Compute missing values up to n. */
    while (cache_size < n) {
        idx_t idx = cache_size + 1;      /* 1‑based index of the next term */

        seq_t val;
        if (idx <= 7) {                  /* should not happen after init */
            /* Fallback to safeguard against future changes. */
            switch (idx) {
                case 1: val = 81;          break;
                case 2: val = 268;         break;
                case 3: val = 928;         break;
                case 4: val = 2784;        break;
                case 5: val = 9628;        break;
                case 6: val = 28912;       break;
                case 7: val = 100008;      break;
                default: val = 0;          break;
            }
        } else {
            /* Recurrence: a(n) = 8*a(n-2) + 24*a(n-4) + 8*a(n-6) */
            val = 8   * cache[idx-3] +   /* a(n-2) -> cache idx-3 */
                  24  * cache[idx-5] +   /* a(n-4) -> cache idx-5 */
                  8   * cache[idx-7];    /* a(n-6) -> cache idx-7 */
        }
        cache[cache_size++] = val;
    }

    return cache[n-1];
}
