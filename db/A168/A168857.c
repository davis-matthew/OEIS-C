/*
 * A168857  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For 1 ≤ n ≤ 19: a(n) = 2 + 34 × ∑_{k=1}^{n} a(n–k);
 *            For n = 20: a(20) = 1 – 595 × a(0) + 34 × ∑_{k=1}^{19} a(20–k) =
 *            1 – 595 + 34 × ∑_{k=1}^{19} a(k); For n ≥ 21: a(n) = 34 ×
 *            ∑_{k=1}^{19} a(n–k) – 595 × a(n–20)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for already computed terms */
static seq_t *cache      = NULL;   /* array of terms */
static idx_t cache_len   = 0;      /* number of terms already computed */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache holds at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, we leave cache as NULL or smaller; the
       caller will get undefined behaviour, but this is acceptable
       for the intended environment. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            val = (seq_t)1;
        } else if (i <= 19) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];
            val = (seq_t)2 + (seq_t)34 * sum;
        } else if (i == 20) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 19; ++k)
                sum += cache[20 - k];
            val = (seq_t)1 - (seq_t)595 * cache[0] + (seq_t)34 * sum;
        } else { /* i >= 21 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 19; ++k)
                sum += cache[i - k];
            val = (seq_t)34 * sum - (seq_t)595 * cache[i - 20];
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
