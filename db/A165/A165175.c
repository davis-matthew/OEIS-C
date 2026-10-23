/*
 * A165175  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=43, a(2)=1806, a(3)=75852, a(4)=3185784,
   a(5)=133802928, a(6)=5619722976, a(7)=236028364992.
   For n ≥ 1:
   a(n) = p_n + 41·(a(n‑1)+…+a(n‑7)) – 861·a(n‑8)
   where p_n = 1 if n==0 or n==8, 2 if 1≤n≤7, otherwise 0. */

#include <stdlib.h>
#include <stddef.h>

/* base values 0…7, cast to seq_t for any underlying type */
static const seq_t base_vals[8] = {
    (seq_t)1ULL,
    (seq_t)43ULL,
    (seq_t)1806ULL,
    (seq_t)75852ULL,
    (seq_t)3185784ULL,
    (seq_t)133802928ULL,
    (seq_t)5619722976ULL,
    (seq_t)236028364992ULL
};

/* cache storage */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  last_cached = -1;  /* no cached values yet */

/* Ensure the cache can hold index `need` (0‑based). */
static void
ensure_capacity(idx_t need)
{
    if (cache_cap > (size_t)need)
        return;
    size_t new_cap = (size_t)need + 1;
    if (new_cap < 8)
        new_cap = 8;
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the p_n value from the generating function. */
static seq_t
p_value(idx_t n)
{
    if (n == 0 || n == 8)
        return (seq_t)1UL;
    if (n <= 7)
        return (seq_t)2UL;
    return (seq_t)0UL;
}

/* Initialise the cache with the first eight known terms. */
static void
init_cache(void)
{
    ensure_capacity(7);
    for (size_t i = 0; i <= 7; ++i)
        cache[i] = base_vals[i];
    last_cached = 7;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* initialise cache if first call */
    if (cache == NULL)
        init_cache();

    /* ensure capacity for index n */
    ensure_capacity(n);

    /* if already computed, return it */
    if (n <= last_cached)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        seq_t val = p_value(i);
        seq_t sum = 0;
        for (int j = 1; j <= 7 && j <= i; ++j)
            sum += cache[i - j];
        val += (seq_t)41 * sum;
        if (i >= 8)
            val -= (seq_t)861 * cache[i - 8];
        cache[i] = val;
    }
    last_cached = n;
    return cache[n];
}
