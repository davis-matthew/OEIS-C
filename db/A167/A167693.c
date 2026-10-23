/*
 * A167693  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from P(t)/Q(t) in the OEIS entry:
   a(0) = 1
   for n ≥ 1
   a(n) = 20 * (a(n−1) + a(n−2) + … + a(n−14))
          − 210 * a(n−15)
          + p(n)
   where
        p(0) = 1,  p(15) = 1,  p(1≤k≤14) = 2
   This recurrence yields all terms of A167693. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamically grown array of terms   */
static idx_t  cache_size = 0;   /* number of terms currently stored   */
static size_t cache_cap  = 0;   /* current allocated capacity         */

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap == 0) {
        cache_cap = 16;                             /* start capacity  */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                         /* allocation failed – caller will get 0 */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cache_size = 0;
    }
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                           /* realloc failed – keep old cache */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (cache_size == 0) {                 /* initialise a(0) */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute terms up to n if not already computed */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= 14; ++j) {
                if (i >= (idx_t)j) sum += cache[i - j];
            }
            seq_t term = sum * (seq_t)20;
            if (i >= 15) term -= cache[i - 15] * (seq_t)210;

            /* add p(i) */
            if (i <= 15) {
                if (i == 0 || i == 15) term += (seq_t)1;
                else                    term += (seq_t)2;
            }
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
