/*
 * A005099  (( Primes == -1 (mod 4) ) + 1)/4.
 * https://oeis.org/A005099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = n-th k such that 4*k - 1 is prime, i.e. (A002145(n) + 1)/4 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t cap = 0;
    static idx_t computed = 0;

    if (n <= computed) {
        return (seq_t)cache[n - 1];
    }

    if (n > cap) {
        idx_t newcap = cap ? cap : 256;
        while (newcap < n) newcap *= 2;
        uint64_t *p = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
        if (!p) return (seq_t)0;
        cache = p;
        cap = newcap;
    }

    uint64_t k = computed ? cache[computed - 1] : 0;
    for (idx_t i = computed + 1; i <= n; i++) {
        do {
            k++;
        } while (!is_prime(4 * k - 1));
        cache[i - 1] = k;
    }
    computed = n;
    return (seq_t)cache[n - 1];
}
