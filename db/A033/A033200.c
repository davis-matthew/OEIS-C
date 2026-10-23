/*
 * A033200  Primes congruent to {1, 3} (mod 8); or, odd primes of form x^2 +
 *          2*y^2.
 * https://oeis.org/A033200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) are the primes p such that p ≡ 1 or 3 (mod 8) (i.e. primes of the form x²+2y²). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined beyond the offset */

    /* static cache of primes congruent to 1 or 3 mod 8 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* generate more terms if needed */
    if (cache_len < n) {
        uint64_t cand = (cache_len == 0) ? 3 : ((uint64_t)cache[cache_len - 1]) + 2;

        while (cache_len < n) {
            /* only numbers ≡ 1 or 3 (mod 8) could be in the sequence */
            if ((cand & 7ULL) == 1 || (cand & 7ULL) == 3) {
                if (is_prime(cand)) {
                    /* extend the cache if full */
                    if (cache_len == cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) abort();       /* allocation failure */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)cand;
                }
            }
            cand += 2;  /* next odd integer */
        }
    }

    return cache[n - 1];
}
