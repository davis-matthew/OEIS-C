/*
 * A164365  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=30, a(3)=150, a(4)=750, a(5)=3750,
 *            a(6)=18750, a(7)=93735 a(n) = 4*∑_{k=1..6} a(n-k) – 10*a(n-7)
 *            for n ≥ 8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache   = NULL;
static idx_t  cache_len   = 0;   /* number of values stored (next free index)   */
static size_t cache_cap   = 0;   /* allocated capacity                         */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed * 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out‑of‑memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call with the 8 known terms */
    if (!seq_cache)
    {
        cache_cap = 16;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache[0] = (seq_t)1;          /* a(0) */
        seq_cache[1] = (seq_t)6;          /* a(1) */
        seq_cache[2] = (seq_t)30;         /* a(2) */
        seq_cache[3] = (seq_t)150;        /* a(3) */
        seq_cache[4] = (seq_t)750;        /* a(4) */
        seq_cache[5] = (seq_t)3750;       /* a(5) */
        seq_cache[6] = (seq_t)18750;      /* a(6) */
        seq_cache[7] = (seq_t)93735;      /* a(7) */
        cache_len = 8;
    }

    if (n < cache_len)
        return seq_cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        /* sum of the six preceding terms a(i-1) … a(i-6) */
        for (idx_t k = 1; k <= 6; ++k)
            sum += seq_cache[i - k];
        seq_t val = sum * (seq_t)4;
        val -= seq_cache[i - 7] * (seq_t)10;
        seq_cache[i] = val;
    }

    cache_len = n + 1;   /* we have now computed up to n */
    return seq_cache[n];
}
