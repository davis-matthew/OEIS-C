/*
 * A097702  a(n) = (A063880(n) - 108)/216.
 * https://oeis.org/A097702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A063880(n) - 108) / 216
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the n‑th nonnegative integer k such that k mod 3 != 1
   and 2*k+1 is squarefree (i.e. contains no squared prime factor). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree_odd.h"

/* static cache for computed k values */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0;
static size_t cache_cap  = 0;
static idx_t  next_k     = 0;

/* expand the cache capacity as needed */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i < cache_sz; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache      = new_arr;
    cache_cap  = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* offset handling */
    size_t target = (size_t)n;

    ensure_cache_cap(target);

    while (cache_sz < target) {
        if (next_k % 3 != 1) {                 /* coprime to 6 condition   */
            uint64_t m = 2 * (uint64_t)next_k + 1;   /* odd element to test     */
            if (is_squarefree_odd(m)) {           /* squarefree check         */
                cache[cache_sz++] = (seq_t)next_k;
            }
        }
        ++next_k;
    }
    return cache[n - 1];
}
