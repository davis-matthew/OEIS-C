/*
 * A093464  a(1) = 1; for n>1, a(n) = n*a(n-1) - 1 if n is a prime else a(n) =
 *          n*a(n-1) + 1.
 * https://oeis.org/A093464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1; for n > 1,
 *   a(n) = n * a(n-1) - 1   if n is prime
 *   a(n) = n * a(n-1) + 1   otherwise
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) for i >= 0 */
    static size_t cap = 0;      /* current capacity of the cache array   */
    static seq_t *cache = NULL; /* pointer to the cache array             */
    static idx_t upper = 0;     /* highest index that has already been computed (0 means none) */

    /* initialise cache on first call */
    if (cap == 0) {
        cap   = 1;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)1; /* a(1) = 1 */
        upper = 1;
    }

    /* grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = mem_resize(seq_t, cache, new_cap);
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = upper + 1; i <= n; ++i) {
        /* a(i-1) is stored at cache[i-2] (i >= 2) */
        seq_t prev  = cache[i - 2];
        seq_t prod  = prev * (seq_t)i;   /* n * a(n-1)   */
        if (is_prime(i))
            cache[i - 1] = prod - (seq_t)1;
        else
            cache[i - 1] = prod + (seq_t)1;
    }

    /* update the highest computed index */
    if (upper < n) upper = n;

    return cache[n - 1];
}
