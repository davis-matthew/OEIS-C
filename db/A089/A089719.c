/*
 * A089719  Pseudofactor sets of primes ending in 1: 9 greater than 3.
 * https://oeis.org/A089719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = the n-th prime p ≡ 1 (mod 10) such that ((p + 8) / 9) %
 *            10 > 3 (i.e., ceil(p/9) ends with a digit 4–9)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1; /* will be incremented to 11 first */

    if (n == 0) return 0;

    if (n <= cache_len) return cache[n-1];

    while (cache_len < n) {
        next_candidate += 10;                    /* candidates ending in 1 */
        if (is_prime_big(next_candidate)) {
            seq_t k = (next_candidate + (seq_t)8) / 9; /* ceil(p/9) */
            if ((k % 10) > 3) {
                if (cache_len >= cache_cap) {
                    ensure_capacity(cache_len + 1, &cache_cap,
                                    (void**)&cache, sizeof(seq_t));
                }
                cache[cache_len++] = next_candidate;
            }
        }
    }
    return cache[n-1];
}
