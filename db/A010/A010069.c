/*
 * A010069  a(n+1) = a(n) + sum of digits in base 7 representation of a(n).
 * https://oeis.org/A010069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_digits_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n+1)=a(n)+sum of digits of a(n) expressed in base 7 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sum_digits_base7.h"

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise cache for n == 0. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
    }

    /* Grow cache to accommodate index n. */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache,
                                 (size_t)cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t s    = sum_digits_base7(prev);
            cache[i]   = prev + s;   /* a(i) = a(i‑1) + sumdigits_base7(a(i‑1)) */
        }
    }

    return cache[n];
}
