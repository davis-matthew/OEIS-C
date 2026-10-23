/*
 * A142034  Primes congruent to 30 mod 31.
 * https://oeis.org/A142034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p % 31 == 30
 * Range:     u8 n <= 1, u16 n <= 217, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;
    static idx_t next_candidate = 61; /* first prime 31*k + 30 */

    if (n == 0) {
        /* undefined behavior for n < 1; return 0 */
        return (seq_t)0;
    }

    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
    }

    while (count < n) {
        if (is_prime((idx_t)next_candidate)) {
            if (count == capacity) {
                capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (tmp == NULL) return (seq_t)0;
                cache = tmp;
            }
            cache[count++] = (seq_t)next_candidate;
        }
        next_candidate += 31;
    }

    return cache[n - 1];
}
