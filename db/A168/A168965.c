/*
 * A168965  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A168965
   Number of reduced words of length n in Coxeter group on 48 generators
   with relations (S_i)^2 = (S_i S_j)^22 = I.

   Recurrence (found from the rational generating function):
     a(n) = 46*(a(n-1)+a(n-2)+...+a(n-21)) - 1081*a(n-22)   for n >= 22
   Initial values computed from the generating function (P(t)/Q(t)):
     a(0) = 1,
     a(n) = 2 + 46 * (a(n-1)+...+a(0))   for 1 <= n <= 21
*/
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;            /* array of computed terms      */
static size_t cache_cap = 0;           /* allocated capacity           */
static size_t cache_len = 0;           /* number of terms stored      */

/* Resize the cache if needed, without changing existing values. */
static void ensure_cache_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;             /* we need indices 0..n inclusive */
    if (need <= cache_cap) return;

    /* Choose a larger new capacity, at least double the old one. */
    size_t new_cap = cache_cap ? cache_cap * 2 : need;
    if (new_cap < need) new_cap = need;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete [] cache;
    cache = tmp;
#else
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

/* Compute a(n) for all n up to the requested index, storing results
   in the static cache. */
static void compute_up_to(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: enough for the first 22 terms and possibly the
           requested index.  This guarantees that we can build the recurrence
           for n >= 22. */
        size_t init_cap = 22;
        if ((size_t)n + 1 > init_cap) init_cap = n + 1;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
#endif
        cache_cap = init_cap;
        cache_len = 0;
    }
    if (n < cache_len) return;   /* already computed */

    /* compute missing values */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i < 22) {
            /* a(i) = P_i + 46 * sum_{k=1..i} a(i-k),  P_0=1, P_i=2 for i>0 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) sum += cache[i - k];
            cache[i] = (seq_t)2 + (seq_t)46 * sum;
        } else {
            /* a(i) = 46*sum_{k=1..21} a(i-k) - 1081*a(i-22) */
            seq_t sum21 = 0;
            for (idx_t k = 1; k <= 21; ++k) sum21 += cache[i - k];
            cache[i] = (seq_t)46 * sum21 - (seq_t)1081 * cache[i - 22];
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache_capacity(n);
    if ((size_t)n >= cache_len) compute_up_to(n);
    return cache[(size_t)n];
}
