/*
 * A224348  Number of nX3 0..2 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A224348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -16*a(n-2) -24*a(n-3) +164*a(n-4) -516*a(n-5)
 *            +140*a(n-6) +956*a(n-7) -1500*a(n-8) +3180*a(n-9) +60*a(n-10)
 *            Initial values: a(1)=10, a(2)=100, a(3)=788, a(4)=5880,
 *            a(5)=45064, a(6)=349280, a(7)=2710892, a(8)=21021916,
 *            a(9)=163012744, a(10)=1264202660
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms */
static seq_t *cache = NULL;
static idx_t cached_n = 0;       /* highest index computed */
static size_t cache_cap = 0;     /* allocated capacity */

static void init_cache(void)
{
    if (cache_cap == 0) {
        /* Initial allocation of some capacity, more will grow as needed */
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* Base values for n = 1 .. 10 */
    const seq_t base[10] = {10, 100, 788, 5880, 45064, 349280, 2710892,
                            21021916, 163012744, 1264202660};
    for (int i = 0; i < 10; ++i)
        cache[i + 1] = base[i];

    cached_n = 10;
}

static void ensure_cache(idx_t n)
{
    if (n <= cached_n)
        return;

    /* Expand cache to hold indices up to n (1‑based) */
    size_t new_cap = (size_t)(n + 1);   /* include index 0 unused */
    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort(); /* memory failure */
        cache = new_cache;
        /* Zero new slots (not strictly necessary, but keeps them clean) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0, but return 0 to avoid UB */
        return (seq_t)0;
    }

    if (cache == NULL)
        init_cache();

    if (n <= cached_n)
        return cache[n];

    /* Compute new terms up to n */
    ensure_cache(n);
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t s_pos = cache[i - 1] * (seq_t)10
                    + cache[i - 4] * (seq_t)164
                    + cache[i - 6] * (seq_t)140
                    + cache[i - 7] * (seq_t)956
                    + cache[i - 9] * (seq_t)3180
                    + cache[i -10] * (seq_t)60;

        seq_t s_neg = cache[i - 2] * (seq_t)16
                    + cache[i - 3] * (seq_t)24
                    + cache[i - 5] * (seq_t)516
                    + cache[i - 8] * (seq_t)1500;

        cache[i] = s_pos - s_neg;
    }
    cached_n = n;
    return cache[n];
}
