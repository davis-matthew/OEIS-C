/*
 * A171416  A sequence with Somos-4 Hankel transform.
 * https://oeis.org/A171416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 1
   a(n) = a(n-2) + Σ_{k=1}^{n-1} a(k-1) * a(n-k-1)   for n ≥ 2
*/

#include <stddef.h>
#include <stdint.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache to store computed terms (thread‑unsafe but meets requirements) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated size in indices */
    static idx_t cache_used = 0;     /* next index to compute */

    /* Allocate or grow cache if needed */
    if (n >= cache_used) {
        idx_t need = n + 1;          /* number of terms required */
        if (cache_cap == 0) {        /* first allocation */
            cache = seq_alloc(need);
            cache_cap = need;
        } else if (need > cache_cap) { /* grow cache */
            cache = seq_realloc(cache, cache_cap, need);
            cache_cap = need;
        }

        /* Compute terms up to n */
        for (idx_t i = cache_used; i < need; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 1;
            } else {
                seq_t sum = cache[i - 2];   /* a(i-2) term */
                for (idx_t j = 0; j <= i - 2; ++j) {
                    sum += cache[j] * cache[i - 2 - j];
                }
                cache[i] = sum;
            }
        }
        cache_used = need;
    }
    return cache[n];
}
