/*
 * A126656  a(1)=1; for n>1, a(n) = Sum_{k=1..n-1} a(k) * floor(n/k).
 * https://oeis.org/A126656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n-1} a(k) * ⌊n/k⌋ , a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 2951 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is computed on demand with memoisation.
    The helper `ensure_cache` is defined in "../utils/ensure_cache.h".
    It guarantees that `cache` has enough capacity for indices up to the
    requested value.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

/* external linkage */
seq_t a(idx_t n)
{
    /* static memoisation table   */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed = 0;   /* largest n already computed */

    if (n == 0)     /* never used by the OEIS offset, but harmless */
        return 0;

    /* resize cache if necessary */
    ensure_cache(n + 1, &cache, &cap);

    /* compute any missing terms up to n */
    for (idx_t j = computed + 1; j <= n; ++j)
    {
        if (j == 1)
        {
            cache[1] = 1;
        }
        else
        {
            seq_t sum = 0;
            for (idx_t k = 1; k < j; ++k)
            {
                sum += cache[k] * (seq_t)(j / k);
            }
            cache[j] = sum;
        }
    }
    computed = n;
    return cache[n];
}
