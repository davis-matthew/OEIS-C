/*
 * A142218  Primes congruent to 21 mod 41.
 * https://oeis.org/A142218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 21 mod 41.
   Computes a(n) by testing numbers of the form 41*k+21 for primality. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t k = 1;  /* last tested k; start with 1 so first test is k=2 */

    if (n == 0) return 0;          /* undefined for offset 0, but safe return */

    /* grow cache if needed */
    while (sz < n) {
        if (sz == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }
        seq_t candidate;
        do {
            k++;
            candidate = ((seq_t)41 * k) + 21;
        } while (!is_prime_big(candidate));
        cache[sz++] = candidate;
    }
    return cache[n - 1];
}
