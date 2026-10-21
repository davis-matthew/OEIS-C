/*
 * A215384  Primes congruent to {1, 2, 3, 6} mod 17.
 * https://oeis.org/A215384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 1, 2, 3, or 6 (mod 17) via incremental search */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = 2;

    if (!primes) {
        cache_cap = 64;
        primes = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!primes) abort();
    }

    while (cache_size < n) {
        seq_t p = next_candidate;

        /* Advance the candidate for the next iteration */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;        /* skip even numbers after 2 */

        /* Prüfen, ob p Primzahl ist und die gewünschte Residenz besitzt */
        if (p == 2 || p % 2 == 1) {          /* p is 2 or an odd number */
            if (isPrimeGeneric(p)) {
                seq_t r = p % 17;
                if (r == 1 || r == 2 || r == 3 || r == 6) {
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap << 1 : 64;
                        seq_t *tmp = (seq_t*)realloc(primes, cache_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        primes = tmp;
                    }
                    primes[cache_size++] = p;
                }
            }
        }
    }

    return primes[n - 1];
}
