/*
 * A167285  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167285: number of reduced words in the Coxeter group
   on 29 generators S_i with (S_i)^2 = I, (S_i S_j)^14 = I.
   Derived from the rational generating function
   (t^14+2t^13+...+2t+1)/(378t^14-27t^13-...-27t+1).
   Recurrence (for n ≥ 15):
       a(n) = 27a(n-1)+27a(n-2)+…+27a(n-13) – 378a(n-14)
   Initial values:
       a(0)=1
       for 1 ≤ n ≤ 13:  a(n)=2 + 27∑_{k=0}^{n-1} a(k)
       a(14)=1 + 27∑_{k=1}^{13} a(k) – 378a(0)   */
#include <stdlib.h>

static seq_t *cache = NULL;          /* storage for computed values       */
static size_t cache_cap = 0;          /* allocated size of cache           */
static int initialized = 0;          /* whether the first 15 terms have
                                       been computed                    */

/* Compute the first 15 terms a(0)…a(14) */
static void init_first_terms(void)
{
    if (initialized) return;
    cache_cap = 15;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)1;
    seq_t sum = cache[0];                 /* sum of a(0)…a(n-1) */
    for (size_t i = 1; i <= 13; ++i) {
        cache[i] = (seq_t)2 + (seq_t)27 * sum;
        sum += cache[i];
    }
    /* a(14) = 1 + 27*(a(13)+…+a(1)) – 378*a(0) */
    seq_t sum1 = sum - cache[0];          /* a(1)+…+a(13)   */
    cache[14] = (seq_t)1 + (seq_t)27 * sum1
                - (seq_t)378 * cache[0];
    initialized = 1;
}

/* Ensure that cache has at least n+1 entries and compute missing
   values using the recurrence for n ≥ 15 */
static void grow_cache(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (cache_cap >= target) return;

    size_t old_cap = cache_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
    cache = new_cache;
    cache_cap = target;

    if (!initialized) init_first_terms();   /* compute a(0)…a(14) if not yet done */

    /* Compute remaining terms */
    for (size_t i = old_cap; i < target; ++i) {
        /* Only compute when i ≥ 15; for 15 ≤ i < 15 we already have data */
        if (i <= 14) continue;
        seq_t val = 0;
        for (int k = 1; k <= 13; ++k) {
            val += (seq_t)27 * cache[i - k];
        }
        val -= (seq_t)378 * cache[i - 14];
        cache[i] = val;
    }
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[(size_t)n];
}
