/*
 * A015305  Gaussian binomial coefficient [ n,5 ] for q = -2.
 * https://oeis.org/A015305
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 -11*(-2)^(n-4) +55*(-2)^(2*n-7) -55*(-2)^(3*n-9)
 *            +11*(-2)^(4*n-10) -(-2)^(5*n-10))/40095 (for n ≥ 5)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 11, i64 n <= 17, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -21 a(n-1) + 462 a(n-2) + 3080 a(n-3)
                 -14784 a(n-4) -21504 a(n-5) +32768 a(n-6)   (n ≥ 11) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* array of computed values, indexed by n */
static idx_t  cache_size = 0;     /* number of slots currently allocated */
static idx_t  last_computed = 0;  /* last n for which a(n) has been stored */

/* Ensure the cache is allocated up to at least (size - 1) */
static void ensure_cache(idx_t size)
{
    if (cache_size >= size)
        return;
    size_t new_size = (size_t)size;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    cache_size = size;
}

/* Initial values a(5) .. a(10) hard‑coded (needed only for recurrence) */
static void init_cache(void)
{
    if (cache_size >= 11 && last_computed >= 10)
        return;                 /* already initialized */

    ensure_cache(11);           /* we need indices 0..10 */

    /* fill indices 0..4 with 0 (unused but harmless) */
    for (idx_t i = 0; i < 5; ++i)
        cache[i] = (seq_t)0;

    cache[5] = (seq_t)1;
    cache[6] = (seq_t)-21;
    cache[7] = (seq_t)903;
    cache[8] = (seq_t)-25585;
    cache[9] = (seq_t)875007;
    cache[10] = (seq_t)-27125217;

    last_computed = 10;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;           /* offset is 5; values for n<5 do not matter */

    init_cache();                  /* guarantees cache[5..10] exist */

    /* If the needed term is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* Extend cache to contain indices up to n */
    ensure_cache(n + 1);           /* allocate n+1 slots so index n is valid */

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Recurrence applies for i ≥ 11; for i = 11..n we compute normally */
        seq_t val = (seq_t)0;
        val += (seq_t)(-21)  * cache[i - 1];
        val += (seq_t)462   * cache[i - 2];
        val += (seq_t)3080  * cache[i - 3];
        val += (seq_t)-14784 * cache[i - 4];
        val += (seq_t)-21504 * cache[i - 5];
        val += (seq_t)32768 * cache[i - 6];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
