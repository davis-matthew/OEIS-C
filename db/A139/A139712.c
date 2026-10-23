/*
 * A139712  a(1)=1. For n >=2, a(n) = largest positive integer < a(n-1) that
 *          is not coprime to n, if such an integer exists. Otherwise, a(n) =
 *          smallest integer > a(n-1) that is not coprime to n.
 * https://oeis.org/A139712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1021, u32 n <= 1021, u64 n <= 1021,
 *            u128 n <= 1021, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A139712:
   a(1) = 1.
   For n ≥ 2, a(n) = largest integer < a(n‑1) that has a common divisor
   with n (i.e. gcd(k, n) > 1).  If no such integer exists,
   a(n) = smallest integer > a(n‑1) that shares a divisor with n.  */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;

    if (n < 1) return 0;            /* undefined for n = 0, never requested */

    /* Initialise cache with a(1) */
    if (cache == NULL) {
        cache_len = 2;                      /* indices 0..1 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;           /* unused, for 1‑based indexing */
        cache[1] = 1;           /* a(1) */
    }

    /* Grow cache and compute missing terms up to n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;      /* keep index n accessible */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            seq_t prev = cache[k - 1];
            seq_t cur = 0;

            /* Search downwards for a smaller integer sharing a factor */
            if (prev > 1) {
                for (seq_t i = prev - 1; i > 0; --i) {
                    if (gcd(i, (seq_t)k) != (seq_t)1) {
                        cur = i;
                        break;
                    }
                }
            }

            /* If none found, search upwards */
            if (cur == 0) {
                seq_t j = prev + 1;
                while (1) {
                    if (gcd(j, (seq_t)k) != (seq_t)1) {
                        cur = j;
                        break;
                    }
                    ++j;
                }
            }

            cache[k] = cur;
        }
        cache_len = new_len;
    }
    return cache[n];
}
