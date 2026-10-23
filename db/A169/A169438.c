/*
 * A169438  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169438 (OEIS)
   Recurrence:
     a(0) = 1
     for n>=1:
       a(n) = 39 * Σ_{k=1}^{31} a(n-k) - 780 * a(n-32) + p(n)
     where
       p(n) = 1   if n==0 or n==32
       p(n) = 2   if 1 <= n <= 31
       p(n) = 0   otherwise
   The recurrence follows from the generating function:
       (1+2∑_{k=1}^{31}t^k + t^32) / (1-39∑_{k=1}^{31}t^k + 780t^32)
*/
#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding values a[0..cache_len-1] */
static idx_t  cache_len = 0;     /* number of values currently computed */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;            /* already computed */

    /* Reallocate cache to hold up to index n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* memory failure */
    cache = new_cache;

    /* Compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 31 && i >= k; ++k)
            sum += cache[i - k];

        seq_t prev32 = (i >= 32) ? cache[i - 32] : 0;

        seq_t p = 0;
        if (i == 0 || i == 32)
            p = 1;
        else if (i >= 1 && i <= 31)
            p = 2;

        cache[i] = 39 * sum - (seq_t)780 * prev32 + p;
    }

    cache_len = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
