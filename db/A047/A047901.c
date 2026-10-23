/*
 * A047901  a(1) = 9; a(n+1) = a(n) * sum of decimal digits of a(n).
 * https://oeis.org/A047901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1) = 9; a(n+1) = a(n) * digit_sum(a(n)) (OEIS A047901)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v); */

static seq_t *cache          = NULL;   /* cache[0] holds a(1) */
static idx_t  cache_capacity = 0;     /* number of elements allocated */
static idx_t  last_computed   = 0;     /* largest index computed (1‑based) */

seq_t a(idx_t n)
{
    if (n < 1)          /* n=0 is undefined, return 0 as a safe placeholder */
        return (seq_t)0;

    /* Grow the cache if needed */
    if (n > cache_capacity) {
        /* Allocate a larger array (no shrinking) */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory – terminate */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);          /* zero‑based offset */
        if (i == 1) {
            cache[idx] = (seq_t)9;
        } else {
            seq_t prev = cache[idx - 1];
            seq_t ds   = digit_sum(prev);
            cache[idx] = prev * ds;
        }
    }
    last_computed = n;

    return cache[n - 1];
}
