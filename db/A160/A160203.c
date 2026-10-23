/*
 * A160203  Positive numbers y such that y^2 is of the form x^2+(x+809)^2 with
 *          integer x.
 * https://oeis.org/A160203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6 a(1)=641, a(2)=809,
 *            a(3)=1105, a(4)=2741, a(5)=4045, a(6)=5989
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache for memoization */
static seq_t *cache = NULL;     /* array of seq_t values, index 0 unused */
static idx_t cache_size = 0;    /* capacity allocated (including index 0) */
static idx_t max_computed = 0;  /* largest n for which cache[n] is valid */

/* initialise cache with the first six terms */
static void init_cache(void)
{
    static const seq_t initial[7] = {0, 641, 809, 1105, 2741, 4045, 5989};
    idx_t need = 7;   /* indices 0 .. 6 */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[need];
#else
        cache = (seq_t *)malloc(need * sizeof(seq_t));
#endif
        cache_size = need;
    } else if (cache_size < need) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[need];
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) exit(1); /* allocation failure */
        cache = tmp;
#endif
        cache_size = need;
    }
    for (idx_t i = 0; i < need; ++i) cache[i] = initial[i];
    max_computed = 6;
}

/* grow cache to accommodate indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    idx_t need = n + 1;   /* +1 because we use 1‑based indexing */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[need];
#else
        cache = (seq_t *)malloc(need * sizeof(seq_t));
#endif
        cache_size = need;
    } else if (cache_size < need) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[need];
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) exit(1); /* allocation failure */
        cache = tmp;
#endif
        cache_size = need;
    }
}

/* compute a(n) using the recurrence and memoisation */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n <= 0 */

    if (cache == NULL) init_cache();  /* cache empty → initialise */

    if (n <= max_computed) return cache[n]; /* already known */

    ensure_capacity(n);  /* make sure the array can hold index n */

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 6 * a(i-3) - a(i-6) for i > 6 */
        seq_t term = (seq_t)6 * cache[i - 3] - cache[i - 6];
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
