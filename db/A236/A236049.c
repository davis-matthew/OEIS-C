/*
 * A236049  Number of (n+1) X (2+1) 0..2 arrays colored with the difference of
 *          the maximum and minimum in each 2 X 2 subblock.
 * https://oeis.org/A236049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 5*a(n-3) + 21*a(n-4) + 12*a(n-5) +
 *            4*a(n-6) (valid for n > 9) Initial values: a(1)=292, a(2)=1068,
 *            a(3)=3516, a(4)=11656, a(5)=38700, a(6)=128920, a(7)=429252,
 *            a(8)=1429352, a(9)=4757532
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;          /* array of computed a(n), 1-based index */
static idx_t cache_cap = 0;          /* allocated capacity (size of array)   */
static idx_t cache_len = 0;          /* largest n that has been computed      */

/* ensure that cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 10;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* memory failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* initialise cache with the first 9 values */
static void init_cache(void)
{
    if (cache)
        return;   /* already initialised */

    ensure_cache(9); /* at least capacity for indices 0..9 */

    /* index 0 unused */
    cache[0] = 0;
    cache[1] = (seq_t)292;
    cache[2] = (seq_t)1068;
    cache[3] = (seq_t)3516;
    cache[4] = (seq_t)11656;
    cache[5] = (seq_t)38700;
    cache[6] = (seq_t)128920;
    cache[7] = (seq_t)429252;
    cache[8] = (seq_t)1429352;
    cache[9] = (seq_t)4757532;
    cache_len = 9;
}

/* public routine requested by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset, but safe to return 0 */

    init_cache();

    if (n <= cache_len)
        return cache[n];

    /* compute up to n, extending cache as needed */
    if (n > cache_cap - 1)
        ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* recurrence only valid for i >= 10; for i <= 9 we already have values */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];
        cache[i] = t1 + ((seq_t)4 * t2) + ((seq_t)5 * t3)
                 + ((seq_t)21 * t4) + ((seq_t)12 * t5) + ((seq_t)4 * t6);
    }
    cache_len = n;
    return cache[n];
}
