/*
 * A142892  Primes congruent to 5 mod 63.
 * https://oeis.org/A142892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 5 modulo 63
 * Range:     u8 n <= 2, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
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
    if (n == 0) return 0;          /* undefined for n=0, safe default */

    /* Cache all found primes of the form 5 mod 63. */
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t capacity = 0;
    static seq_t candidate = 5;     /* first member to test */

    while (len < n) {
        if (is_prime((idx_t)candidate)) {
            if (len == capacity) {
                capacity = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!tmp) abort();     /* allocation failed */
                cache = tmp;
            }
            cache[len++] = candidate;
        }
        candidate += 63;
    }

    return cache[n - 1];
}
