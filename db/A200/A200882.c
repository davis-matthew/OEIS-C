/*
 * A200882  Number of 0..4 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -10*a(n-2) +20*a(n-3) -15*a(n-4) +21*a(n-5)
 *            -7*a(n-6) +8*a(n-7) -a(n-8) +a(n-9) (n ≥ 10) a(1) = 95, a(2) =
 *            325, a(3) = 1121, a(4) = 3985, a(5) = 14288, a(6) = 50995, a(7)
 *            = 181336, a(8) = 644721, a(9) = 2294193
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache: cache[n] = a(n) */
static size_t cache_cap = 0;     /* allocated capacity (>= n+1) */
static size_t cached_max = 0;    /* largest n whose value has been computed */

/* Ensure the array can index up to n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n + 2;           /* +2 for safety */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* out of memory */
    cache = tmp;
    /* initialise newly allocated entries to 0 */
    for (size_t i = old_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Initialise the first nine terms, if not already done. */
static void init_base(void)
{
    if (cached_max >= 9)
        return;                              /* already initialised */
    ensure_cache_cap(9);
    cache[1] = 95;
    cache[2] = 325;
    cache[3] = 1121;
    cache[4] = 3985;
    cache[5] = 14288;
    cache[6] = 50995;
    cache[7] = 181336;
    cache[8] = 644721;
    cache[9] = 2294193;
    cached_max = 9;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for n < 1 */

    init_base();

    if (n <= cached_max)
        return cache[(size_t)n];

    /* extend cache up to n */
    ensure_cache_cap(n);

    for (size_t i = cached_max + 1; i <= (size_t)n; ++i)
    {
        seq_t pos = cache[i-1]*5
                  + cache[i-3]*20
                  + cache[i-5]*21
                  + cache[i-7]* 8
                  + cache[i-9];
        seq_t neg = cache[i-2]*10
                  + cache[i-4]*15
                  + cache[i-6]* 7
                  + cache[i-8];
        cache[i] = pos - neg;
    }
    cached_max = (size_t)n;
    return cache[(size_t)n];
}
