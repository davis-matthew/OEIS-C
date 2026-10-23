/*
 * A163977  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0)=1, a(1)=21, a(2)=420, a(3)=8400, a(4)=168000, *
 *            a(5)=3360000, a(6)=67199790, *
 *            a(n)=19*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) – 190*a(n-6) for
 *            n≥7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;          /* memoised sequence values      */
static size_t cache_used = 0;        /* number of values already in cache */

/* Ensure that cache contains values up to index n inclusive. */
static void compute_up_to(idx_t n)
{
    /* Initial cache with the first seven terms. */
    if (cache == NULL) {
        const size_t init = 7;                                    /* 0 … 6 */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)420;
        cache[3] = (seq_t)8400;
        cache[4] = (seq_t)168000;
        cache[5] = (seq_t)3360000;
        cache[6] = (seq_t)67199790;
        cache_used = init;
    }

    /* If we already have enough values, nothing to do. */
    if (n < (idx_t)cache_used) return;

    /* Grow the cache to the required length. */
    size_t new_used = (size_t)n + 1;
    if (new_used > cache_used) {
        seq_t *tmp = (seq_t*)realloc(cache, new_used * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_used; i <= (size_t)n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            seq_t term = sum * (seq_t)19;
            term -= (seq_t)190 * cache[i-6];
            cache[i] = term;
        }
        cache_used = new_used;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
