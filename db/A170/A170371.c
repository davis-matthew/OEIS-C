/*
 * A170371  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence of the Coxeter growth series with
 *            * generators S_i (i=1..26) and relations S_i^2 = (S_i S_j)^43 =
 *            1. * Let a(0)=1, a(1)=26, a(2)=650, a(3)=16250, a(4)=406250,
 *            a(5)=10156250, * a(6)=253906250, a(7)=6347656250,
 *            a(8)=158691406250, a(9)=3967285156250, * a(10)=99182128906...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoization table for a(n) */
static size_t cache_size = 0; /* number of values currently stored */
static size_t cache_cap   = 0; /* allocated capacity of cache */

/* Ensure the cache can hold values up to index 'n' inclusive. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t needed = (size_t)n + 1;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence described in the comment. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_t val;
        /* n_i coefficient from numerator N(t) */
        if (i == 0 || i == 43)
            val = (seq_t)1;
        else if (i >= 1 && i <= 42)
            val = (seq_t)2;
        else
            val = (seq_t)0;

        /* Sum of the last up to 42 terms */
        seq_t sum_prev = (seq_t)0;
        for (idx_t k = 1; k <= 42; ++k)
        {
            if (i < (size_t)k) break;
            sum_prev += cache[i - k];
        }
        val += (seq_t)24 * sum_prev;

        if (i >= 43)
            val -= (seq_t)300 * cache[i - 43];

        cache[i] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
