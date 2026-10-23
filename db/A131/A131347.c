/*
 * A131347  a(1)=1. a(n) = a(n-1) + (sum of the terms, from among terms a(1)
 *          through a(n-1), which are coprime to sum{k=1 to n-1} a(k)).
 * https://oeis.org/A131347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * a(n) = a(n-1) + Σ a(k) (for 1 ≤ k < n) where
 *            gcd(a(k), S_{n-1}) = 1 * with S_{n-1} = Σ_{i=1}^{n-1} a(i). * *
 *            Authors note: this implementation caches all computed terms and
 *            * uses a straightforward O(n²) algorithm. For n up to 10 000 it
 *            * stays well below the time limit on the target...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* static cache variables ---------------------------------------------- */
static seq_t *cache      = NULL;   /* array of computed a(k) values   */
static size_t cache_size = 0;     /* number of terms actually stored */
static size_t cache_cap  = 0;     /* current capacity of the array    */
static seq_t sum_total   = 0;     /* Σ a(i) for i < (next index)     */

/* Ensure the cache can hold at least `need` terms */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* the actual sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range but harmless            */

    /* one‑time initialization -------------------------------------- */
    if (cache_size == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;  /* a(1) = 1                         */
        cache_size = 1;
        sum_total = cache[0]; /* sum of terms up to a(1)          */
    }

    /* if the requested term is already known ------------------------ */
    if ((size_t)n <= cache_size) return cache[n-1];

    /* extend the cache up to index n --------------------------------*/
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* sum_{1..i-1} is already in `sum_total`                      */
        seq_t cop_sum = 0;
        for (idx_t j = 0; j < i - 1; ++j) {
            if (gcd(cache[j], sum_total) == (seq_t)1) {
                cop_sum += cache[j];
            }
        }

        /* a(i) = a(i-1) + cop_sum */
        seq_t new_term = cache[i - 2] + cop_sum;   /* i >= 2 always here   */

        ensure_capacity((size_t)i);                /* grow cache if needed */
        cache[i - 1] = new_term;

        sum_total += new_term;                     /* update running sum   */
        cache_size++;                              /* one more term stored */
    }

    return cache[n-1];
}
