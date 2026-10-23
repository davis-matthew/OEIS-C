/*
 * A140742  Primes congruent to 73 or 137 mod 210.
 * https://oeis.org/A140742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 73 or 137 (mod 210), with
 *            a(1) = 73. This implementation searches for primes on demand,
 *            caching results to support arbitrary order of calls
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;    /* cached primes that satisfy the congruence */
    static size_t capacity = 0;
    static size_t size = 0;
    static idx_t last_checked = 1; /* last candidate that was tested */

    /* Ensure the cache has at least n elements */
    while (size < n) {
        /* Find next candidate */
        idx_t candidate = last_checked + 1;
        /* Skip even numbers after 2 */
        if (candidate <= 2) candidate = 2;
        else if ((candidate & 1) == 0) candidate++;

        /* Test candidates until we find n primes */
        while (size < n) {
            if ((candidate % 210 == 73) || (candidate % 210 == 137)) {
                if (is_prime(candidate)) {
                    /* Grow cache if needed */
                    if (size == capacity) {
                        size_t new_cap = capacity == 0 ? 16 : capacity * 2;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) return 0; /* allocation error; fallback */
                        cache = tmp;
                        capacity = new_cap;
                    }
                    cache[size++] = (seq_t)candidate;
                }
            }
            last_checked = candidate;
            candidate++;
            /* Skip even numbers */
            if (candidate > 2 && (candidate & 1) == 0) candidate++;
        }
    }

    return cache[n - 1];
}
