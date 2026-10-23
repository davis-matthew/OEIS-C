/*
 * A156566  a(2n+2) = 9*a(2n+1), a(2n+1) = 9*a(2n) - 8^n*A000108(n), a(0)=1.
 * https://oeis.org/A156566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   catalan, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A156566
 * Recurrence:
 *   a(0) = 1
 *   a(2n+2) = 9 * a(2n+1)
 *   a(2n+1) = 9 * a(2n) - 8^n * Catalan(n)   // Catalan numbers = A000108
 *
 * The function caches computed terms so that calls may appear in any order.
 */

#include <stdlib.h>                 /* realloc, exit */
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/catalan.h"       /* seq_t catalan(idx_t k) */

/* Static cache for memoization (externally visible across calls). */
static seq_t *cache = NULL;         /* a(0) .. a(k) */
static idx_t cache_size = 0;        /* number of stored elements */

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    /* Expand the cache if the requested index is beyond the current size. */
    if (n >= cache_size) {
        /* Allocate or grow the storage. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            exit(1);                 /* out‑of‑memory on demand */
        }
        cache = tmp;

        /* Initialise the zeroth element if the cache was empty. */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;     /* a(0) = 1 */
            cache_size = 1;
        }

        /* Compute all missing entries in strictly increasing order. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if ((i & 1ULL) == 0ULL) { /* even index: a(2m) = 9 * a(2m-1) */
                cache[i] = cache[i - 1] * (seq_t)9;
            } else {                 /* odd index: a(2m+1) = 9*a(2m) - 8^m*Catalan(m) */
                idx_t m = (i - 1) >> 1;               /* m = (i-1)/2 */
                seq_t pow8 = pow_uint(8ULL, m);        /* 8^m */
                seq_t cat   = catalan(m);              /* Catalan(m) */
                cache[i] = cache[i - 1] * (seq_t)9 - pow8 * cat;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
