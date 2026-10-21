/*
 * A057627  Number of nonsquarefree numbers not exceeding n.
 * https://oeis.org/A057627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - Sum_{k=1}^n mu(k)^2 (number of nonsquarefree integers
 *            ≤ n)
 * Range:     u8 n <= 655, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *prefix = NULL;
    static idx_t cache_n = 0;
    static uint8_t *sqfree = NULL;

    if (n > cache_n) {
        size_t sz = (size_t)n + 1;

        uint8_t *new_sqfree = (uint8_t*)malloc(sz);
        seq_t *new_prefix  = (seq_t*)malloc(sz * sizeof(seq_t));
        assert(new_sqfree != NULL && new_prefix != NULL);

        /* Initially assume all numbers ≥ 1 are square‑free. */
        for (size_t i = 1; i < sz; ++i)
            new_sqfree[i] = 1;
        new_sqfree[0] = 0;

        /* Sieve primes up to √n */
        idx_t lim = _floor_isqrt_u64(n);
        size_t l   = (size_t)lim;
        uint8_t *is_prime = (uint8_t*)malloc(l + 1);
        assert(is_prime != NULL);
        for (size_t i = 2; i <= l; ++i) is_prime[i] = 1;

        for (size_t i = 2; i <= l; ++i) {
            if (!is_prime[i]) continue;
            for (size_t j = i * i; j <= l; j += i)
                is_prime[j] = 0;

            uint64_t sq = (uint64_t)i * (uint64_t)i;
            for (uint64_t j = sq; j <= n; j += sq)
                new_sqfree[(size_t)j] = 0;
        }
        free(is_prime);

        /* Build prefix sums of square‑free counts */
        seq_t cumulative = 0;
        new_prefix[0] = 0;
        for (size_t i = 1; i < sz; ++i) {
            if (new_sqfree[i]) cumulative++;
            new_prefix[i] = cumulative;
        }

        free(sqfree);
        free(prefix);
        sqfree = new_sqfree;
        prefix = new_prefix;
        cache_n = n;
    }

    /* Number of nonsquare‑free integers ≤ n = n – (# of square‑free ≤ n) */
    return (seq_t)n - prefix[(size_t)n];
}
