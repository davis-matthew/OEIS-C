/*
 * A206348  Smallest number k such that the decimal representations of k^2 and
 *          (k+n)^2 are anagrams.
 * https://oeis.org/A206348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_frequencies, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: find the smallest k such that
   k² and (k+n)² are digit anagrams for a given offset n (1‑based). */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* malloc, realloc, free */
#include "../utils/digit_sum.h"
#include "../utils/digit_frequencies.h"

/* static cache for memoisation of results (1‑based indices) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least 'n' entries (1‑based). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;                /* double until big enough */

    /* Reallocate the cache array.  Cast `void*` to the class pointer. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap != 0)          /* allocation failure */
        exit(EXIT_FAILURE);
    cache = tmp;
    /* initialise new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* defensive: OEIS offset is 1 */
        return 0;

    ensure_cache_size(n);
    if (cache[n - 1] != 0)    /* already computed */
        return cache[n - 1];

    idx_t k = 1;
    seq_t sq1 = (seq_t)1 * (seq_t)1;                    /* k²   */
    seq_t sq2 = (seq_t)(1 + n) * (seq_t)(1 + n);        /* (k+n)² */
    unsigned int freq1[10], freq2[10];

    for (;;) {
        /* Quick filter: digit sums (mod 9) must be equal. */
        if (digit_sum(sq1) == digit_sum(sq2)) {
            digit_frequencies(sq1, freq1);
            digit_frequencies(sq2, freq2);
            int same = 1;
            for (int d = 0; d < 10; ++d)
                if (freq1[d] != freq2[d]) { same = 0; break; }
            if (same) {
                cache[n - 1] = (seq_t)k;
                return cache[n - 1];
            }
        }

        /* Increment the squares efficiently:
           (k+1)² = k² + 2k + 1,  (k+n+1)² = (k+n)² + 2(k+n) + 1. */
        seq_t inc1 = (seq_t)2 * (seq_t)k + 1;
        seq_t inc2 = (seq_t)2 * (seq_t)(k + n) + 1;
        sq1 += inc1;
        sq2 += inc2;
        ++k;
    }
}
