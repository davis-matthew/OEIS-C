/*
 * A134336  Nonnegative integers n containing each digit between n's smallest
 *          and largest decimal digits.
 * https://oeis.org/A134336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) computed by brute‑force search for n‑th integer whose
 *            decimal digits form a consecutive set
 * Range:     u8 n <= 60, u16 n <= 3191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_consecutive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_consecutive.h"

/*
 * Sequence generator.
 * Maintains a static cache so that calls are fast even for arbitrary order.
 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* cache of a(1)...a(k) */
    static size_t  cache_cap = 0;         /* allocated capacity */
    static size_t  cache_len = 0;         /* number of values stored */
    static seq_t   next_candidate = 0;    /* next integer to test */

    if (n == 0)                /* guard against 0 index (offset is 1) */
        return 0;

    /* generate until we have at least n values */
    while (cache_len < n) {
        if (is_consecutive(next_candidate)) {
            if (cache_len == cache_cap) {
                /* grow cache */
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – terminate. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];  /* 1‑based indexing */
}
