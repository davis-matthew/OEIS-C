/*
 * A103871  Numbers n such that 100n + 69 is prime.
 * https://oeis.org/A103871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9762 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 100·n + 69 is prime (OEIS A103871) */
/* Computation: iterate n, test primality of 100·n+69, cache the
   satisfying n's. The cache is grown dynamically on demand,
   so calls in any order are handled efficiently. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;   /* cache of found n's */
    static idx_t terms_size = 0;  /* number of cached terms */
    static idx_t terms_cap   = 0; /* allocated capacity */
    static idx_t next_candidate = 1; /* next n to test */

    /* Grow cache until we have at least N terms */
    while (terms_size < n) {
        /* Test current candidate */
        uint64_t val = 100ULL * (uint64_t)next_candidate + 69ULL;
        if (is_prime_u64(val)) {
            /* Ensure capacity */
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap << 1 : 128;
                seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                if (!tmp) return 0; /* malloc failure; return 0 as fallback */
                terms = tmp;
            }
            terms[terms_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* Return the Nth term (offset 1) */
    return terms[n - 1];
}
