/*
 * A088304  a(1) = 1. For n > 1, a(n) = a(n-1) if n is prime, a(n) = a(n-1)/n
 *          if n is composite and divides a(n-1) else a(n) = n*a(n-1).
 * https://oeis.org/A088304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 47, u64 n <= 99, u128 n <= 206,
 *            big any n
 * Helpers:   is_prime_inline
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088304.  For n ≥ 1:
 * a(1) = 1.
 * For n > 1:
 *   if n is prime            → a(n) = a(n-1);
 *   else if a(n-1) divisible by n  → a(n) = a(n-1) / n;
 *   else                     → a(n) = a(n-1) * n.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_inline.h"

/* Static cache: cache[i] holds a(i) for i ≥ 1 (cache[0] unused).  */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest index stored in the cache */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined; return 0 safely */
        return 0;

    /* Initialise cache on first call.  */
    if (cache == NULL) {
        cache = (seq_t*)calloc(2, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[1] = 1;                  /* a(1) = 1 */
        cache_size = 1;
    }

    /* Grow cache and compute missing terms up to n.  */
    if (n > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if (is_prime_inline(i)) {
                cache[i] = prev;               /* prime → unchanged */
            } else {
                if (prev % (seq_t)i == 0) {     /* divisible → divide */
                    cache[i] = prev / (seq_t)i;
                } else {                       /* otherwise multiply */
                    cache[i] = prev * (seq_t)i;
                }
            }
        }
        cache_size = n;
    }

    return cache[n];
}
