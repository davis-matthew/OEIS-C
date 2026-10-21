/*
 * A224664  Number of n X n 0..7 matrices with each 2X2 subblock idempotent.
 * https://oeis.org/A224664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5)
 *            -3*a(n-6) +6*a(n-7) -a(n-9) for n > 10 Initial values: a(1)=8,
 *            a(2)=32, a(3)=200, a(4)=318, a(5)=700, a(6)=1292, a(7)=2476,
 *            a(8)=4570, a(9)=8406, a(10)=15256 The function memoises computed
 *            values and returns the correct term for any n ≥ 1...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based storage, index 0 unused */
static size_t cache_size = 0; /* largest index computed */
static size_t cache_cap  = 0; /* capacity of allocated array */

static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[1] = 8;
    cache[2] = 32;
    cache[3] = 200;
    cache[4] = 318;
    cache[5] = 700;
    cache[6] = 1292;
    cache[7] = 2476;
    cache[8] = 4570;
    cache[9] = 8406;
    cache[10] = 15256;
    cache_size = 10;
}

/* Ensure that the cache can hold at least one index > 'need' (1‑based). */
static void ensure_capacity(size_t need)
{
    while (cache_cap <= need) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined below offset – return 0 */
        return 0;

    init_cache();

    size_t idx = (size_t)n;
    if (idx <= cache_size)
        return cache[idx];

    for (size_t i = cache_size + 1; i <= idx; ++i) {
        ensure_capacity(i);
        /* compute a(i) using the recurrence; all ops are seq_t */
        seq_t val = 0;
        val = val + ((seq_t)6  * cache[i-1]);
        val = val - ((seq_t)12 * cache[i-2]);
        val = val + ((seq_t)5  * cache[i-3]);
        val = val + ((seq_t)12 * cache[i-4]);
        val = val - ((seq_t)12 * cache[i-5]);
        val = val - ((seq_t)3  * cache[i-6]);
        val = val + ((seq_t)6  * cache[i-7]);
        val = val -        cache[i-9];
        cache[i] = val;
    }

    cache_size = idx;
    return cache[idx];
}
