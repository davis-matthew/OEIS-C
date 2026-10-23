/*
 * A169279  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A169279:
   a(0)=1;
   for 1≤n≤28:  a(n)=2+24*∑_{i=max(0,n-28)}^{n-1} a(i);
   for n≥29:    a(n)=24*∑_{i=n-28}^{n-1} a(i) – 300*a(n-29)   */

/* The same cache is reused for all calls. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms   */
static size_t cache_cap = 0;    /* allocated capacity        */
static idx_t  cache_len = 0;    /* number of terms cached    */

/* Ensure space for at least 'cap' elements. */
static void
ensure_capacity(size_t cap)
{
    if (cache_cap < cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);   /* out of memory */
        cache     = new_cache;
        cache_cap = cap;
    }
}

seq_t
a(idx_t n)
{
    /* Allocate space for indices up to n. */
    ensure_capacity((size_t)n + 1);

    /* Fill missing values up to n. */
    if (cache_len <= n) {
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t sum = 0;
            /* Determine the starting index of the sum. */
            idx_t start;
            if (k <= 28) {
                /* For initial terms, include all known previous terms. */
                start = (k > 28) ? k - 28 : 0;
                /* Coefficient from the numerator: 1 for k=0, 2 otherwise. */
                seq_t coeff = (k == 0 ? (seq_t)1 : (seq_t)2);
                /* Sum of a[start .. k-1] */
                for (idx_t j = start; j < k; ++j)
                    sum += cache[j];
                /* a(k) = coeff + 24*sum */
                cache[k] = coeff + (seq_t)24 * sum;
            } else {
                /* k ≥ 29: use recurrence derived from the denominator. */
                start = k - 28;
                for (idx_t j = start; j < k; ++j)
                    sum += cache[j];
                /* a(k) = 24*sum - 300*a(k-29) */
                cache[k] = (seq_t)24 * sum - (seq_t)300 * cache[k - 29];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
