/*
 * A111369  Numbers k such that 13*k + 11 is prime.
 * https://oeis.org/A111369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th k such that 13*k + 11 is prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size   = 0;
    static size_t cap    = 0;
    static idx_t last_k  = 0;

    if (n == 0) return (seq_t)0;  /* Undefined for n<1, but return 0 */

    while (size < n) {
        uint64_t k = (uint64_t)last_k;
        uint64_t candidate = 13ULL * k + 11ULL;
        if (is_prime(candidate)) {
            if (size == cap) {
                size_t new_cap = cap ? (cap << 1) : 8;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = (seq_t)k;
        }
        last_k++;
    }
    return cache[n - 1];
}
