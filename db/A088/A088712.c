/*
 * A088712  Primes which when concatenated with the previous prime form a new
 *          prime.
 * https://oeis.org/A088712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   compute A088712: primes which when concatenated with the
 *            previous prime form a new prime
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   concat_parts, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined below offset; return 0 */

    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static state for prime generation */
    static uint64_t prev_prime = 2;
    static uint64_t curr_prime = 3;
    static int initialized = 0;

    if (!initialized) {
        prev_prime = 2;
        curr_prime = 3;
        cache_len = 0;
        cache_cap = 0;
        initialized = 1;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t next_prime_val = next_prime(curr_prime);
        seq_t concat = concat_parts(next_prime_val, curr_prime);
        if (is_prime_big(concat)) {
            /* ensure enough capacity */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap <= cache_len) new_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_prime_val;
        }
        /* shift primes for next iteration */
        prev_prime = curr_prime;
        curr_prime = next_prime_val;
    }

    return cache[n - 1];
}
