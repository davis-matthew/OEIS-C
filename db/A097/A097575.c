/*
 * A097575  A 2 X 2 matrix Fibonacci sequence.
 * https://oeis.org/A097575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-4) + a(n-8)   for n ≥ 8,
 * with initial values
 * a(0)=1, a(1)=1, a(2)=1, a(3)=2,
 * a(4)=1, a(5)=2, a(6)=2, a(7)=3.
 *
 * The function caches computed terms so each value is produced
 * only once regardless of call order.
 */

#include <stdlib.h>   /* malloc, realloc, calloc */

static seq_t *cache = NULL;          /* array holding computed terms */
static idx_t cache_size = 0;         /* allocated size of cache array */
static idx_t max_computed = (idx_t)-1;/* highest n for which cache[n] is valid */

/* Ensure the cache can access indices up to n */
static void ensure_capacity(idx_t n)
{
    if (cache != NULL && cache_size > n)
        return;

    idx_t newsize = cache_size ? cache_size * 2 : 8;
    while (newsize <= n)
        newsize <<= 1;

    if (cache == NULL) {
        /* first allocation: zero-filled so values can be overwritten */
        seq_t *tmp = (seq_t *)calloc(newsize, sizeof(seq_t));
        cache = tmp;
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        cache = tmp;
    }
    cache_size = newsize;
}

/* Build the first eight terms using Fibonacci numbers */
static void build_base_values(void)
{
    seq_t fib[9];
    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= 8; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    for (idx_t i = 0; i <= 7; ++i) {
        idx_t q = i / 4;
        idx_t r = i % 4;
        seq_t val;
        if (r == 0)
            val = fib[q + 1];
        else if (r == 1 || r == 2)
            val = fib[q + 2];
        else /* r == 3 */
            val = fib[q + 3];
        cache[i] = val;
    }
    max_computed = 7;
}

seq_t a(idx_t n)
{
    /* First time: allocate cache and build the base values */
    if (cache == NULL) {
        ensure_capacity(7);     /* enlarges cache to hold at least indices 0..7 */
        build_base_values();
    }

    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n */
    ensure_capacity(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 4] + cache[i - 8];
    }
    max_computed = n;
    return cache[n];
}
