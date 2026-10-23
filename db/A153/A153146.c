/*
 * A153146  Numbers n such that 2*n + 19 and 2*n - 19 are prime.
 * https://oeis.org/A153146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153146: a(n) = n such that 2*n + 19 and 2*n – 19 are prime.
   Computed by incremental search, caching results. */
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t test = 1;          /* next integer to test */

    if (!cache) {
        cap = 128;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    while (len < (size_t)n) {
        /* find next qualifying integer */
        for (;;) {
            idx_t cand = test++;
            idx_t p1 = 2 * cand + 19;
            idx_t p2 = 2 * cand - 19;

            if (p2 <= 1)
                continue;           /* 2*n-19 must be > 1 */

            if (is_prime(p1) && is_prime(p2)) {
                /* store result */
                if (len >= cap) {
                    cap *= 2;
                    cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = (seq_t)cand;
                break;
            }
        }
    }
    return cache[n - 1];
}
