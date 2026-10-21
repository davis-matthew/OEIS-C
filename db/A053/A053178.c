/*
 * A053178  Numbers ending in 1 which are not prime.
 * https://oeis.org/A053178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n-th non‑prime integer of the form 10k + 1 (including 1). */
/* a(1)=1, a(2)=21, a(3)=51, a(4)=81, … */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* static cache of computed terms */
static seq_t *cache    = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;
static idx_t next_k     = 0;   /* next k to test, 0 → candidate 1 */

/* ensure that the cache can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cache = (seq_t *)tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    if (cache_size < n) {
        ensure_capacity(n);
        while (cache_size < n) {
            uint64_t candidate = next_k * 10ULL + 1ULL;   /* 10k+1 */
            if (!is_prime(candidate)) {                   /* non‑prime (or 1) */
                cache[cache_size++] = (seq_t)candidate;
            }
            next_k++;                                     /* advance k */
        }
    }
    return cache[n - 1];                 /* 1‑based indexing */
}
