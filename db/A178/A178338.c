/*
 * A178338  Numbers k such that the sum of decimal digits of k does not divide
 *          k+1.
 * https://oeis.org/A178338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer k such that the sum of decimal digits of k
   does NOT divide k+1.  The sequence starts with a(1)=2, a(2)=3, …   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    /* static state for caching the found numbers */
    static idx_t last_tested = 1;          /* largest k tested so far   */
    static size_t seen = 0;                /* number of values cached   */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (seen < n) {
        if (cache.empty()) cache.reserve(64);
    }
#else
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
#endif

    while (seen < n) {
        ++last_tested;
        seq_t k = (seq_t)last_tested;

        /* compute digit sum of k */
        unsigned int freq[10];
        digit_frequencies(k, freq);
        seq_t sum = 0;
        for (int d = 0; d < 10; ++d) sum += (seq_t)d * freq[d];

        if (sum == 0) continue;          /* should never happen for k ≥ 1 */

        seq_t mod = (k + 1) % sum;
        if (mod != 0) {
#ifdef __cplusplus
            if (cache.size() == seen) cache.push_back(k);
#else
            if (seen >= cache_capacity) {
                size_t new_cap = (cache_capacity == 0) ? 64 : cache_capacity * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_capacity = new_cap;
            }
            cache[seen] = k;
#endif
            ++seen;
        }
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
