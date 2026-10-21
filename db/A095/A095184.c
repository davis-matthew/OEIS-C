/*
 * A095184  a(1) = 3; for n > 1 a(n) is the least prime of form a(n-1) +
 *          k*prime(n-1) with k > 0.
 * https://oeis.org/A095184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; for n>1 a(n) is the least prime of the form
 *            a(n-1)+k*prime(n-1) with k>0. Here prime(j) denotes the j‑th
 *            prime (prime(1)=2)
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[0] stores a(1) */
    static idx_t cache_size = 0;         /* number of values cached */

    if (n == 0) return 0;                /* undefined, but safe */

    /* grow the cache if we need more values */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            exit(EXIT_FAILURE);          /* out of memory */
        }
        cache = new_cache;

        /* compute all missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {                /* a(1) = 3 */
                cache[0] = 3;
                continue;
            }

            /* previous term a(i-1) */
            seq_t prev = cache[i - 2];

            /* prime at index (i-1) in the prime sequence: get_prime is 1‑based */
            uint32_t p = get_prime(i - 1);   /* prime(i-1) */

            seq_t step = (seq_t)p;
            seq_t candidate;

            for (idx_t k = 1; ; ++k) {
                candidate = prev + (seq_t)k * step;
                if (is_prime_big(candidate)) {
                    cache[i - 1] = candidate;
                    break;
                }
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
