/*
 * A167460  Primes p such that (p reversed)+6 is also a prime.
 * https://oeis.org/A167460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primes p for which reverse(p) + 6 is also prime
 * Range:     u8 n <= 13, u16 n <= 286, u32 n <= 286, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache */
    static size_t len   = 0;   /* number of primes stored */
    static idx_t  next  = 2;   /* next integer to test */

    while (len < (size_t)n) {
        idx_t p = next++;

        /* skip even numbers greater than 2 */
        if (p > 2 && (p & 1) == 0)
            continue;

        if (!is_prime_big((seq_t)p))
            continue;

        /* decimal reverse of p */
        idx_t revp = 0;
        idx_t temp = p;
        while (temp > 0) {
            revp = revp * 10 + (temp % 10);
            temp /= 10;
        }

        seq_t candidate = (seq_t)revp + (seq_t)6;
        if (!is_prime_big(candidate))
            continue;

        /* store the found prime */
        if (len == cap) {
            size_t new_cap = cap ? cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len++] = (seq_t)p;
    }

    return cache[n - 1];
}
