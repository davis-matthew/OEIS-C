/*
 * A081203  9th binomial transform of (0,1,0,1,0,1,.....), A000035.
 * https://oeis.org/A081203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 8^n)/2 (recurrence a(n)=18*a(n-1)-80*a(n-2),
 *            a(0)=0, a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;         /* array of computed terms          */
static idx_t  cache_size = 0;       /* size of the allocated array      */
static idx_t  computed   = 0;       /* number of terms actually filled */

/* initialise cache with a(0) and a(1) */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed   = 2;
    }
}

/* enlarge the cache so that index n is valid */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = cache_size ? cache_size : 2;
    while (new_size <= n)
        new_size <<= 1;                 /* grow exponentially */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache)
        exit(1);                        /* fatal in case of out‑of‑memory */
    cache = new_cache;
    cache_size = new_size;
}

/* Compute the n‑th term of sequence A081203 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    init_cache();
    if (n < computed)
        return cache[n];

    ensure_capacity(n);
    /* compute terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* recurrence: a(i) = 18*a(i-1) - 80*a(i-2) */
        cache[i] = (seq_t)(18 * cache[i-1] - 80 * cache[i-2]);
    }
    computed = n + 1;
    return cache[n];
}
