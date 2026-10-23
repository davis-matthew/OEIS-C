/*
 * A045457  Primes congruent to {0, 4} mod 5.
 * https://oeis.org/A045457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 0 or 4 (mod 5)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
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
    if (n == 0) return 0;  /* undefined, but safe */

    /* static cache of the found primes */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of primes stored */
    static uint64_t candidate = 9;  /* next number to test (5k+4, k=2) */

    /* one-time initialization */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)5;   /* a(1) */
        len = 1;
        candidate = 9;         /* start testing 5k+4 numbers, k=2 */
    }

    /* grow cache until it contains at least n primes */
    while (len < n) {
        /* test candidate; it is always of form 5k+4, k>=2 */
        if (is_prime((idx_t)candidate)) {
            if (len == cap) {          /* double capacity when needed */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)candidate;
        }
        candidate += 5;  /* next candidate with p ≡ 4 (mod 5) */
    }

    return cache[n - 1];
}
