/*
 * A165788  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 10 * 9^(n-1) for 1 ≤ n ≤ 9 a(10) = 10 * 9^9 - 45
 *            (all words of length 10 minus the 45 alternating pairs) a(n) =
 *            8*(a(n-1)+…+a(n-9)) - 36*a(n-10) for n ≥ 11 (See the rational
 *            generating function in OEIS A165788.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* stores computed values   */
static size_t cache_len = 0;      /* highest index stored + 1 */
static size_t cache_cap = 0;      /* size of allocated memory */

/* compute 9^k for non‑negative k */
static seq_t pow9(idx_t k)
{
    seq_t v = (seq_t)1;
    for (idx_t i = (idx_t)0; i < k; ++i)
        v = v * (seq_t)9;
    return v;
}

/* ensure cache has capacity at least need */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < need)
            new_cap = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();               /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* initialise cache with first 11 terms (indices 0..10) */
static void init_cache(void)
{
    ensure_cache_cap(11);
    cache[0] = (seq_t)1;                     /* a(0) */
    for (idx_t n = 1; n <= 9; ++n)            /* a(1)..a(9) */
        cache[n] = (seq_t)10 * pow9(n - 1);
    cache[10] = (seq_t)10 * pow9(9) - (seq_t)45; /* a(10) */
    cache_len = 11;
}

seq_t a(idx_t n)
{
    /* initialise if first call */
    if (!cache)
        init_cache();

    /* already computed? */
    if (n < cache_len)
        return cache[n];

    /* extend cache to n */
    ensure_cache_cap(n + 1);
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 9; ++k)
            sum += cache[i - k];
        /* a(i) = 8*sum - 36*a(i-10)   (relevant for i ≥ 11) */
        cache[i] = (seq_t)8 * sum - (seq_t)36 * cache[i - 10];
    }
    cache_len = n + 1;
    return cache[n];
}
