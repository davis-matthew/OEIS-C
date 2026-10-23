/*
 * A170026  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 17*16^(n-1) for 1 ≤ n ≤ 35 For n ≥ 36 the
 *            sequence satisfies the linear recurrence a(n) =
 *            15*(a(n-1)+…+a(n-35)) – 120*a(n-36)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Grow the cache vector so that cache[n] is valid */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* grow capacity exponentially */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);               /* insufficient memory */
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise missing entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 35) {          /* free‑group formula */
            if (i == 1)
                cache[i] = (seq_t)17;
            else
                cache[i] = cache[i - 1] * (seq_t)16;
        } else {                      /* recurrence */
            seq_t sum35 = 0;
            for (idx_t j = 1; j <= 35; ++j)
                sum35 += cache[i - j];
            cache[i] = (seq_t)15 * sum35 - (seq_t)120 * cache[i - 36];
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
