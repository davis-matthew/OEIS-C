/*
 * A130763  Natural numbers such that d(n)!+ 1 is a square, where d(n) is the
 *          number of divisors of n, A000005.
 * https://oeis.org/A130763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer such that d(n)! + 1 is a perfect square
   (equivalently, the n‑th integer whose divisor count τ(n) is 4, 5, or 7) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* cache holding the computed terms of the sequence */
static seq_t *cache      = NULL;
static size_t cache_size = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* capacity of the cache array */
static idx_t  next_check  = 1;     /* next integer to test for inclusion */

/* ensure the cache can hold at least 'need' terms */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        /* In C++ a C‑style cast is allowed to convert the void* from realloc
           to the typed pointer seq_t*. */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure, terminate */
        cache_cap = new_cap;
    }
}

/* return the n-th term of the sequence (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n <= 0 */
    while (cache_size < (size_t)n) {      /* generate terms until we have n of them */
        uint64_t d = divisor_count_u64((uint64_t)next_check);
        if (d == 4u || d == 5u || d == 7u) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return cache[n - 1];
}
