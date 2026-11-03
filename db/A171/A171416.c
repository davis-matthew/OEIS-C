/*
 * A171416  A sequence with Somos-4 Hankel transform.
 * https://oeis.org/A171416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 995 (2026-11-01)
 * Helpers:   none
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

#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
