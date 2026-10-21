/*
 * A240778  Number of n X 3 0..1 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A240778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A240778
    Formula: a(n) = 5*a(n-1) - a(n-2) - a(n-3) + 4*a(n-4)
               - 4*a(n-5) - 3*a(n-6) + a(n-7);
    initial values:
        a(1) = 1
        a(2) = 4
        a(3) = 20
        a(4) = 97
        a(5) = 459
        a(6) = 2187
        a(7) = 10442
*/

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;     /* 1‑based indexing: cache[n] = a(n) */
static size_t cache_cap = 0;    /* allocated size (>= n+1) */
static idx_t   cache_last = 0;  /* largest n for which a(n) is known */

/* ensure the cache can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
    while (new_cap <= n)
        new_cap <<= 1;                 /* grow geometrically */

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* initialise first seven values once, when the cache is grown the first time */
    if (cache_last < 7) {
        /* value at index 0 is unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)97;
        cache[5] = (seq_t)459;
        cache[6] = (seq_t)2187;
        cache[7] = (seq_t)10442;
        cache_last = 7;
    }
}

/* main routine: compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 */

    ensure_cache(n);

    /* return cached value if already computed */
    if (n <= cache_last)
        return cache[n];

    /* compute all missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* compute the recurrence */
        seq_t res = 5 * cache[i - 1];
        res += 4 * cache[i - 4];
        res += cache[i - 7];

        /* compute the subtraction part */
        seq_t sub = cache[i - 2];
        sub += cache[i - 3];
        seq_t t4 = 4 * cache[i - 5];
        sub += t4;
        seq_t t3 = 3 * cache[i - 6];
        sub += t3;

        /* safe subtraction, result is non‑negative by definition */
        res -= sub;

        cache[i] = res;
    }
    cache_last = n;
    return cache[n];
}
