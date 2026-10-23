/*
 * A170718  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170718
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

/* Sequence A170718: number of reduced words of length n in a Coxeter group
   with 37 generators and relations (S_i)^2 = (S_i S_j)^50 = I.
   Formula used: for all n ≥ 0
        a(n) = N_n + 35 * Σ_{k=1}^{min(n,49)} a(n-k) - 630 * a(n-50)   (a(n-50)=0 if n<50)
   where the numerator coefficients are
        N_0 = 1,  N_n = 2 for 1 ≤ n ≤ 49,  N_50 = 1,  N_n = 0 for n > 50. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* memoisation array   */
static size_t cache_cap = 0;      /* current allocated capacity   */
static size_t cache_len = 0;      /* number of computed terms */

/* Ensure that values 0..n are computed and cached.  */
static void cache_ensure(size_t n)
{
    if (cache_len > n) return;          /* already ready */

    /* grow the cached array if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t N = 0;                     /* numerator coefficient N_i */
        if (i == 0 || i == 50) N = (seq_t)1;
        else if (i <= 49) N = (seq_t)2;

        /* sum of the previous up to 49 terms */
        seq_t sum = 0;
        size_t limit = (i < 49 ? i : 49);
        for (size_t k = 1; k <= limit; ++k) sum += cache[i - k];

        /* main recurrence */
        seq_t val = sum * (seq_t)35;
        if (i >= 50) val -= cache[i - 50] * (seq_t)630;
        val += N;
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    cache_ensure((size_t)n);
    return cache[(size_t)n];
}
