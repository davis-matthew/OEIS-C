/*
 * A086651  a(1)=1, a(2)=1 and for n > 2, a(n) is the smallest positive
 *          integer such that the third-order absolute difference gives the
 *          Fibonacci numbers A000045 = {1,1,2,3,5,8,...}.
 * https://oeis.org/A086651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 18*a(n-6) - 18*a(n-7) - a(n-12) + a(n-13) for
 *            n>15 (initial values: a(1)=1, a(2)=1, a(3)=2, a(4)=5, a(5)=5,
 *            a(6)=10, a(7)=23, a(8)=23, a(9)=2, a(10)=57, a(11)=57,
 *            a(12)=146, a(13)=379, a(14)=379, a(15)=2)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build */
extern seq_t a(idx_t n);

/* Cache for computed terms */
static seq_t *cache = NULL;
static idx_t  cache_cap   = 0;
static idx_t  cache_size  = 0;

/* Initial 15 terms (index 0 unused) */
static const seq_t init[16] = {
    0ULL, /* dummy 0 for index 0 */
    1ULL, /* a(1) */
    1ULL, /* a(2) */
    2ULL, /* a(3) */
    5ULL, /* a(4) */
    5ULL, /* a(5) */
    10ULL,/* a(6) */
    23ULL,/* a(7) */
    23ULL,/* a(8) */
    2ULL, /* a(9) */
    57ULL,/* a(10) */
    57ULL,/* a(11) */
    146ULL,/* a(12) */
    379ULL,/* a(13) */
    379ULL,/* a(14) */
    2ULL  /* a(15) */
};

/* Ensure cache has been computed up to index n (inclusive) */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_size >= n)
        return;                 /* already computed */

    /* Allocate or grow buffer as necessary */
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL)
            exit(1);
        cache_cap = needed;
    } else if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);
        cache = tmp;
        cache_cap = needed;
    }

    /* Set zero term for index 0 (unused) */
    if (cache_size == 0)
        cache[0] = (seq_t)0;

    /* Fill missing initial values up to index 15 */
    idx_t limit = (n < 15) ? n : (idx_t)15;
    for (idx_t i = cache_size + 1; i <= limit; ++i)
        cache[i] = init[i];

    /* Compute remaining terms using the recurrence */
    idx_t start = (cache_size < 16) ? 16 : cache_size + 1;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)18 * cache[i-6]
                 - (seq_t)18 * cache[i-7]
                 - cache[i-12]
                 + cache[i-13];
    }

    cache_size = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_cache_up_to(n);
    return cache[n];
}
