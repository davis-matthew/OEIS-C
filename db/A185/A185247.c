/*
 * A185247  Expansion of 3F2( (1/4,1/2,3/4); (4,6) )(256 x).
 * https://oeis.org/A185247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 188 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1
 *   for n ≥ 1:
 *     n*(n+5)*(n+3)*a(n) = 8*(4*n-3)*(2*n-1)*(4*n-1)*a(n-1)
 *
 * Formula (for reference):
 *   a(n) = 720 * (4n)! / (n!^2 * (n+3)! * (n+5)!)
 *
 * The implementation memoizes computed terms in a static array and
 * uses gcd reductions to avoid overflow when multiplying.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

seq_t
a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  last_n = 0;   /* largest index computed and stored */
    static size_t cache_cap = 0;/* allocated capacity */

    /* Allocate cache for a(0) if needed */
    if (cache == NULL) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        last_n = 0;
    }

    /* Handle a(0) quickly */
    if (n == 0)
        return cache[0];

    /* Extend the cache up to n if required */
    if (n > last_n) {
        /* ensure capacity for indices 0 … n */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t k = last_n + 1; k <= n; ++k) {
            /* compute numerator: 8*(4k-3)*(2k-1)*(4k-1) */
            seq_t num = (seq_t)8;
            num *= (seq_t)(4 * k - 3);
            num *= (seq_t)(2 * k - 1);
            num *= (seq_t)(4 * k - 1);

            /* compute denominator: k*(k+5)*(k+3) */
            seq_t den = (seq_t)k;
            den *= (seq_t)(k + 5);
            den *= (seq_t)(k + 3);

            /* Reduce with gcd to keep numbers small and keep division exact */
            seq_t g1 = gcd(cache[k - 1], den);
            seq_t a_div = cache[k - 1] / g1;
            den = den / g1;

            seq_t g2 = gcd(num, den);
            seq_t num_red = num / g2;
            den = den / g2;

            /* final computation: a(k) = a_div * num_red / den */
            cache[k] = a_div * num_red / den;
        }

        last_n = n;
    }

    /* return already computed value */
    return cache[n];
}
