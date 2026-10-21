/*
 * A235634  Primes whose base-4 representation is also the base-7
 *          representation of a prime.
 * https://oeis.org/A235634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 740, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base4_to_base7, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235634: primes p such that the base‑4 representation of p, when
 * read as a base‑7 number, is also prime.  Computed by incremental search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/base4_to_base7.h"

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 3;        /* next odd number to test after 2 */

/* Append a term to the cache, expanding storage if necessary. */
static void append_cache(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for offset 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t candidate;

        if (cache_len == 0) {   /* handle the prime 2 specially */
            candidate = 2;
            if (isPrimeGeneric(candidate)) {
                seq_t q = base4_to_base7(candidate);
                if (isPrimeGeneric(q))
                    append_cache(candidate);
            }
            next_candidate = 3;
            continue;
        }

        candidate = next_candidate;

        if (isPrimeGeneric(candidate)) {
            seq_t q = base4_to_base7(candidate);
            if (isPrimeGeneric(q))
                append_cache(candidate);
        }

        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    return cache[n - 1];
}
