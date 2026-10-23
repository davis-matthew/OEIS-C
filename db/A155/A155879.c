/*
 * A155879  a(0) = 4; for n > 0, a(n) is the smallest composite number c >
 *          a(n-1) such that c - n is also composite.
 * https://oeis.org/A155879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate to find the smallest composite `c` > a(n‑1)
   such that `c‑n` is also composite.  The sequence is memoized so
   calls can come in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t len = 0;   /* number of terms computed */

    /* lazy allocation of the cache */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* compute terms up to n if needed */
    while (len <= n) {
        if (len == 0) {
            /* a(0) = 4 */
            cache[len++] = (seq_t)4;
            continue;
        }

        /* grow the array if necessary */
        if (len >= cap) {
            size_t newcap = cap << 1;
            seq_t *newptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (newptr == NULL) {
                /* allocation failed – return zero (should not happen) */
                return (seq_t)0;
            }
            cache = newptr;
            cap = newcap;
        }

        seq_t prev = cache[len - 1];
        seq_t candidate = prev + (seq_t)1;

        /* search for the next valid composite */
        while (1) {
            /* skip 0,1,2,3 – they are not composite */
            if (candidate <= (seq_t)3) {
                candidate = candidate + (seq_t)1;
                continue;
            }

            /* candidate must be composite */
            if (is_prime_big(candidate)) {
                candidate = candidate + (seq_t)1;
                continue;
            }

            /* diff = candidate – n (n == len) must be composite */
            seq_t diff = candidate - (seq_t)len;
            if (diff <= (seq_t)1 || is_prime_big(diff)) {
                candidate = candidate + (seq_t)1;
                continue;
            }

            /* found a valid term */
            cache[len++] = candidate;
            break;
        }
    }

    return cache[n];
}
