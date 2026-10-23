/*
 * A164070  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = -630*a(n-6) + 35*∑_{k=1..5} a(n-k)  for n ≥ 7,
 * a(6) = -630*a(0) + 35*∑_{k=1..5} a(6-k) + 1.
 * 
 * Initial values:
 * a(0) = 1
 * a(1) = 37
 * a(2) = 1332
 * a(3) = 47952
 * a(4) = 1726272
 * a(5) = 62145792
 * 
 * The sequence is computed with memoisation. A static cache of size
 * MAX_CACHE (≥ 10000) stores the values. For indices > MAX_CACHE a
 * ring buffer of the last six values is used, so that the recurrence
 * can still be evaluated. The code works for any integer type supplied
 * as seq_t, including arbitrary‑precision integer classes.
 */

#include <stddef.h>   /* for size_t */

/* Should be large enough for n up to at least 10 000 */
#define MAX_CACHE 100000

static seq_t cache[MAX_CACHE + 1];
static idx_t cache_len = 5;   /* highest index currently stored in cache */
static int cache_init   = 0;  /* flag whether the cache has been initialised */

/* initialise the first six terms of the sequence */
static void init_cache(void)
{
    if (cache_init)
        return;
    cache[0] = 1;
    cache[1] = 37;
    cache[2] = 1332;
    cache[3] = 47952;
    cache[4] = 1726272;
    cache[5] = 62145792;
    cache_init = 1;
}

/* compute terms up to up_to (inclusive).  up_to <= MAX_CACHE. */
static void compute_up_to(idx_t up_to)
{
    init_cache();
    if (up_to <= cache_len)
        return;
    for (idx_t i = cache_len + 1; i <= up_to; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 5; ++k)
            sum += (seq_t)35 * cache[i - k];
        if (i == 6u)                /* special formula for a(6) */
            cache[i] = sum - (seq_t)630 * cache[0] + 1;
        else
            cache[i] = sum - (seq_t)630 * cache[i - 6];
    }
    cache_len = up_to;
}

/* the main sequence function */
seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n <= 5) {
        init_cache();
        return cache[(size_t)n];
    }

    /* if index is within the static cache, use it */
    if (n <= (idx_t)MAX_CACHE) {
        compute_up_to(n);
        return cache[(size_t)n];
    }

    /* for very large n use a ring buffer of the last six values.
     * The ring is kept in static current_ring variables so that
     * successive calls can reuse the already computed data. */
    static seq_t ring[6];
    static int ring_inited = 0;         /* flag whether ring has been initialised */

    if (!ring_inited) {
        /* initialise ring with the first six known values */
        for (int i = 0; i < 6; ++i)
            ring[i] = cache[i];
        ring_inited = 1;
    }

    /* compute terms from the current cache_len+1 up to n, using the ring. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 5; ++k)
            sum += (seq_t)35 * ring[(i - k) % 6];
        seq_t val;
        if (i == 6u)
            val = sum - (seq_t)630 * ring[0] + 1;
        else
            val = sum - (seq_t)630 * ring[(i - 6) % 6];
        ring[i % 6] = val;
        /* also store in the static cache if still inside its bounds */
        if (i <= (idx_t)MAX_CACHE)
            cache[(size_t)i] = val;
    }

    cache_len = (idx_t)MAX_CACHE;   /* we filled all indices we can store */
    return ring[n % 6];
}
