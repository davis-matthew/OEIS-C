/*
 * A178389  Multiples of 3 in A175461.
 * https://oeis.org/A178389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A007522(n) where A007522(n) is the nth prime congruent
 *            to 7 mod 8
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    static idx_t *prime_cache = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap   = 0;
    static idx_t next_candidate = 7;   /* next odd number ≡ 7 (mod 8) */

    if (n == 0) return 0;   /* offset begins at 1 */

    while (prime_count < n) {
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_cache = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        } else if (prime_count >= prime_cap) {
            prime_cap <<= 1;
            prime_cache = (idx_t *)realloc(prime_cache, prime_cap * sizeof(idx_t));
        }

        if (is_prime(next_candidate)) {
            prime_cache[prime_count++] = next_candidate;
        }
        next_candidate += 8;
    }

    seq_t result = (seq_t)prime_cache[n - 1];
    result = result * 3;
    return result;
}
