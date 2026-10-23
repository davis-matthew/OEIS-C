/*
 * A170713  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170713
   Formula: a(n) = 30*∑_{k=1}^{49} a(n‑k) – 465*a(n‑50) + p(n),
   where p(0)=1, p(1…49)=2, p(50)=1, and p(n)=0 for n>50. */

#include <stdlib.h>

/* Cache for memoised values. */
static seq_t *cache = NULL;        /* allocated array of seq_t */
static size_t cache_alloc = 0;     /* current allocation size (≥ cache_used) */
static size_t cache_used = 0;      /* number of already computed terms */

/* Return the n-th term a(n). */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold the requested index. */
    if (cache_alloc <= (size_t)n) {
        size_t new_size = cache_alloc > 0 ? cache_alloc * 2 : 16;
        while (new_size <= (size_t)n) {
            new_size *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_alloc = new_size;
    }

    /* Compute any terms that have not yet been memoised. */
    size_t limit = (size_t)n;
    for (size_t i = cache_used; i <= limit; ++i) {
        seq_t sum = 0;

        /* 30 * Σ_{k=1}^{49} a(i-k), skipping negative indices. */
        for (size_t k = 1; k <= 49; ++k) {
            if (i >= k) {
                sum += (seq_t)30 * cache[i - k];
            }
        }

        /* – 465 * a(i‑50) if i ≥ 50. */
        if (i >= 50) {
            sum -= (seq_t)465 * cache[i - 50];
        }

        /* Add the numerator term p(i). */
        seq_t p = 0;
        if (i == 0) {
            p = 1;
        } else if (i == 50) {
            p = 1;
        } else if (i <= 49) {
            p = 2;
        }
        cache[i] = sum + p;
    }

    cache_used = limit + 1;
    return cache[(size_t)n];
}
