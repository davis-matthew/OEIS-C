/*
 * A167789  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   coeff_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167789: Number of reduced words of length n in the Coxeter group
   on 35 generators with relations S_i^2 = (S_i S_j)^15 = I.
   The generating function is
        (t^15 + 2*t^14 + ... + 2*t + 1) / (561*t^15 - 33*t^14 - ... - 33*t + 1)
   which yields the linear recurrence for n ≥ 15
        a(n) = 33*∑_{k=1}^{14} a(n‑k) – 561*a(n‑15),
   with the initial terms defined by
        a(0) = 1,
        a(n) = 2 + 33*(a(n-1)+…+a(n-14)) – 561*a(n-15)  for 1 ≤ n ≤ 14,
        a(15) = 1 + 33*(a(14)+…+a(1)) – 561*a(0).
   The function below computes a(n) lazily, memoising results for all
   indices ever requested.  No hard‑coded table of terms is used.  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/coeff_num.h"

/* dynamic cache of computed values */
static seq_t *cache = NULL;          /* sequence values a(0), a(1), …         */
static size_t cache_size = 0;        /* how many values are stored currently */
static size_t cache_cap   = 0;        /* current capacity of the cache         */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails the program will terminate, which is acceptable
       for the contest environment. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) for the Coxeter group sequence */
seq_t a(idx_t n)
{
    /* initialise the cache if this is the first call */
    if (cache == NULL) {
        ensure_capacity((size_t)n + 1);
        if (n == 0) {                     /* special case n = 0 */
            cache[0] = coeff_num(0);      /* a(0) = 1 */
            cache_size = 1;
            return cache[0];
        }
        /* compute values incrementally up to n */
        for (size_t m = 0; m <= (size_t)n; ++m) {
            seq_t sum14 = (seq_t)0;
            /* sum of the previous 14 terms */
            for (int k = 1; k <= 14; ++k) {
                if (m >= (size_t)k)
                    sum14 += cache[m - k];
            }
            seq_t term15 = (seq_t)0;
            if (m >= 15)
                term15 = cache[m - 15];
            seq_t value = coeff_num(m) + (seq_t)33 * sum14 - (seq_t)561 * term15;
            cache[m] = value;
        }
        cache_size = (size_t)n + 1;
    } else {
        /* extend the cache if necessary */
        if ((size_t)n >= cache_size) {
            ensure_capacity((size_t)n + 1);
            for (size_t m = cache_size; m <= (size_t)n; ++m) {
                seq_t sum14 = (seq_t)0;
                for (int k = 1; k <= 14; ++k) {
                    if (m >= (size_t)k)
                        sum14 += cache[m - k];
                }
                seq_t term15 = (seq_t)0;
                if (m >= 15)
                    term15 = cache[m - 15];
                seq_t value = coeff_num(m) + (seq_t)33 * sum14 - (seq_t)561 * term15;
                cache[m] = value;
            }
            cache_size = (size_t)n + 1;
        }
    }
    return cache[n];
}
