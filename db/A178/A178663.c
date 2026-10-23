/*
 * A178663  a(1)=1. For n>1, a(n) is the smallest number greater than a(n-1)
 *          such that exactly one of n and a(n) is prime and the other is
 *          composite.
 * https://oeis.org/A178663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1)=1; for n>1, a(n) is the smallest number > a(n-1)
   such that exactly one of n and a(n) is prime and the other is composite. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* cached values: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) /* offset not used, but protect against 0 */
        return (seq_t)0;

    if (n > cache_len) {
        /* extend cache size to n (1‑based) */
        seq_t *newcache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!newcache) {
            /* allocation failed – abort, but sequence requested only up to 10000 */
            abort();
        }
        cache = newcache;

        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i == 0) {            /* a(1) */
                cache[0] = (seq_t)1;
                continue;
            }

            idx_t idx = i + 1;        /* current n */
            seq_t prev = cache[i - 1];
            seq_t cand = prev + 1;

            bool nPrime = is_prime_big((seq_t)idx);

            /* search for the smallest candidate > prev satisfying the prime/composite rule */
            while (1) {
                bool candPrime = is_prime_big(cand);
                if (nPrime ^ candPrime) {
                    cache[i] = cand;
                    break;
                }
                cand++;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
