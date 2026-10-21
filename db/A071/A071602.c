/*
 * A071602  Sum of the reverses of the first n primes.
 * https://oeis.org/A071602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} reverse_decimal(get_prime(k))
 * Range:     u8 n <= 9, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Static cache for cumulative sums a(0..computed) */
    static seq_t *cache = NULL;
    static idx_t computed = 0;

    if (n == 0) {
        return 0;        /* undefined in OEIS but safe */
    }

    if (n > computed) {
        /* Expand cache to accommodate index n */
        size_t needed = (size_t)n + 1; /* include a(0) */
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) {
            abort();  /* memory allocation failure */
        }
        cache = new_cache;

        /* Initialize base case if not yet set */
        if (computed == 0) {
            cache[0] = 0;
        }

        /* Compute new cumulative sums */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prime = (seq_t)get_prime(i);
            seq_t rev   = reverse_decimal(prime);
            cache[i] = cache[i - 1] + rev;
        }
        computed = n;
    }

    return cache[n];
}
