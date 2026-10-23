/*
 * A030665  Smallest nontrivial extension of n which is prime.
 * https://oeis.org/A030665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = min{ n*10^k + d | k>=1, 0<=d<10^k, prime }
 * Range:     u8 n <= 19, u16 n <= 661, u32 n <= 1275, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache of computed values (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) return 0;   /* offset is 1 */

    /* enlarge cache if needed */
    if (cache == NULL || n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, n);
        /* initialize new slots */
        for (size_t i = cache_cap; i < n; ++i) cache[i] = 0;
        cache_cap = n;
    }

    /* return memoised result if present */
    if (cache[n-1] != 0) return cache[n-1];

    /* search for the smallest prime extension */
    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t pow10k = pow10(k);        /* 10^k as seq_t */
        seq_t base   = (seq_t)n * pow10k;  /* n * 10^k */

        /* iterate over all d in [0, 10^k-1] */
        /* pow10k fits in 64 bits for all practical k needed */
        uint64_t limit = (uint64_t)pow10k;
        for (uint64_t d = 0; d < limit; ++d) {
            seq_t candidate = base + (seq_t)d;
            if (is_prime_big(candidate)) {
                result = candidate;
                goto found;
            }
        }
    }

found:
    cache[n-1] = result;
    return result;
}
