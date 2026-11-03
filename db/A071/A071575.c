/*
 * A071575  Number of iterations of cototient(n) needed to reach 1
 *          (cototient(n) = n-phi(n)).
 * https://oeis.org/A071575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n - φ(n)) + 1, a(1) = 0 (cototient iterations)
 * Range:     u8 n <= 1246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9556 (2026-11-03)
 * Helpers:   mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* make sure the cache can hold indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise a(1) */
    cache[1] = 0;

    /* compute values in increasing order */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t phi_i = totient_seq((seq_t)i);      /* φ(i) */
        idx_t j = i - (idx_t)phi_i;            /* i - φ(i) < i */
        cache[i] = cache[j] + 1;               /* recurrence */
    }

    return cache[n];
}
