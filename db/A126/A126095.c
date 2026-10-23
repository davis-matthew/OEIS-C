/*
 * A126095  Lesser of pairs of single (non-twin) primes.
 * https://oeis.org/A126095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126095: Lesser of consecutive primes that are single (non‑twin) primes; computed via prime generation and selection. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/next_prime.h"

static inline bool is_single_prime(uint64_t p)
{
    /* return true if p-2 and p+2 are NOT prime (single prime) */
    if (p > 2 && isPrimeGeneric((seq_t)(p - 2))) return false;
    if (isPrimeGeneric((seq_t)(p + 2))) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_prime = 0;      /* last prime found */
    static uint64_t second_last = 0;     /* prime before last */

    /* generate terms until we have at least n */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_prime + 1);
        second_last = last_prime;
        last_prime = p;

        if (second_last != 0) {
            /* pair (second_last, last_prime) are consecutive primes */
            if (is_single_prime(second_last) && is_single_prime(last_prime)) {
                if (cache_len + 1 > cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)second_last;
            }
        }
    }

    return cache[(size_t)n - 1];
}
