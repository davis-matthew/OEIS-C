/*
 * A118397  Eigenvector of triangle A105070, where A105070(n,k) =
 *          2^k*C(n+1,2*k+1) for 0<=k<=[n/2], n>=0.
 * https://oeis.org/A118397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=0..floor(n/2)} 2^k * C(n+1, 2k+1) * a(k)
 *            (n >= 0) (C is the binomial coefficient)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom_n_k, pow2, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom_n_k.h"
#include "../utils/pow2.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* number of allocated elements */
static size_t cache_size = 0;     /* number of elements that have been
                                    computed (cache[0..cache_size-1]) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    if ((size_t)n + 1 <= *cap_ptr) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache;
    if (*cache_ptr == NULL) {
        new_cache = seq_alloc(new_cap);
    } else {
        new_cache = seq_realloc(*cache_ptr, *cap_ptr, new_cap);
    }
    if (!new_cache) return;       /* allocation failed – caller will
                                    receive whatever was computed so far */
    *cache_ptr = new_cache;
    *cap_ptr = new_cap;
}

seq_t a(idx_t n)
{
    /* grow cache if necessary */
    ensure_capacity(n, &cache, &cache_cap);
    /* compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            idx_t ni = (idx_t)i;
            idx_t maxk = ni / 2;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t term = cache[(size_t)k];
                term *= pow2(k);
                seq_t bin = binom_n_k(ni + 1, 2 * k + 1);
                term *= bin;
                sum += term;
            }
            cache[i] = sum;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
