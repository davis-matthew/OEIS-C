/*
 * A061766  a(1) = 4; a(n) = smallest composite number of the form k*a(n-1) +
 *          1.
 * https://oeis.org/A061766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4; a(n) = smallest composite of the form k*a(n-1)+1
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 229, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061766 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;   /* memoization array */
static size_t cache_len = 0;  /* number of elements stored */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; avoid UB */
        return 0;

    /* Expand cache to hold at least n entries */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)                 /* allocation failure – unlikely */
            return 0;
        cache = tmp;

        /* Initialise the first term if the cache was previously empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)4; /* a(1) = 4 */
            cache_len = 1;
        }

        /* Compute missing terms up to n */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            seq_t k = 1;
            seq_t cand;

            for (;;) {
                cand = prev * k + (seq_t)1;
                if (!isPrimeGeneric(cand)) {  /* composite or 1 (impossible here) */
                    cache[i] = cand;
                    break;
                }
                ++k;
            }
        }

        cache_len = new_len;
    }

    return cache[n - 1];
}
