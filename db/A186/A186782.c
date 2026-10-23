/*
 * A186782  Primes of the form k*6^m-1 with k prime and m an integer.
 * https://oeis.org/A186782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form k*6^m - 1 with k prime and m ≥ 0
   (OEIS A186782).                                          */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;  /* candidate to start searching from */

    if (n == 0) return (seq_t)0; /* unspecified for offset zero */

    /* Generate terms up to the required index */
    while (cached < n) {
        /* Start searching from the next candidate value */
        seq_t candidate;
        if (next_candidate < 2)
            candidate = 2;
        else
            candidate = next_candidate;

        /* Find the next prime candidate */
        do {
            if (!is_prime_big(candidate))
                candidate += (candidate == 2 ? 1 : 2); /* skip evens after 2 */
        } while (!is_prime_big(candidate));

        /* Update next_candidate for subsequent searches */
        next_candidate = candidate + ((candidate == 2) ? 1 : 2);

        /* Test whether candidate satisfies the form k*6^m - 1 */
        seq_t t = candidate + 1;               /* t = p + 1 */
        idx_t v2 = 0, v3 = 0;
        seq_t tmp = t;

        /* Count powers of 2 */
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            v2++;
        }
        /* Count powers of 3 */
        while (tmp % 3 == 0) {
            tmp /= 3;
            v3++;
        }
        idx_t m = (v2 < v3) ? v2 : v3;        /* exponent of 6 */

        /* Compute 6^m as the divisor */
        seq_t denom = 1;
        for (idx_t i = 0; i < m; ++i)
            denom *= 6;

        /* k = (candidate + 1) / 6^m */
        seq_t k = (candidate + 1) / denom;

        if (is_prime_big(k)) {
            /* Ensure cache capacity and store the term */
            ensure_capacity(cached + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cached++] = candidate;
        }
    }

    return cache[n - 1];
}
