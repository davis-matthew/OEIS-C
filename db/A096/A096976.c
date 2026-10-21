/*
 * A096976  Number of walks of length n on P_3 plus a loop at the end.
 * https://oeis.org/A096976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) with initial values a(0)=1,
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 41, u64 n <= 79, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoised dynamic programming, computing
   terms up to the largest requested index. */

#include <stdlib.h>
#include <stdint.h>  /* for uintptr_t etc. */

/* Forward declaration of helper functions */
static void init_cache(void);
static void ensure_computed(idx_t n);

/* Static cache for previously computed terms */
static seq_t *cache = NULL;      /* array holding a(n) */
static size_t cache_len = 0;     /* allocated size of cache */
static idx_t  max_comp = 0xFFFFFFFFFFFFFFFFULL;  /* largest n that has been computed */

/* Initialise the cache with the first three terms */
static void
init_cache(void)
{
    if (cache != NULL)
        return;                 /* already initialised */

    cache_len = 3;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (cache == NULL) abort(); /* panic on OOM */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    max_comp = 2;
}

/* Grow the cache if needed, then compute all new terms up to n */
static void
ensure_computed(idx_t n)
{
    init_cache();

    if (n <= max_comp)
        return;                 /* already done */

    /* Allocate enough space: grow exponentially */
    size_t need = (size_t)(n + 1);
    if (need > cache_len) {
        size_t new_len = cache_len;
        while (new_len < need) new_len <<= 1;  /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();             /* panic on OOM */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 2*a(i-2) - a(i-3) */
        cache[i] = cache[i - 1]
                 + cache[i - 2] + cache[i - 2]
                 - cache[i - 3];
    }
    max_comp = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
