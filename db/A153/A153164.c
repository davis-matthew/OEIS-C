/*
 * A153164  Primes p with property that p^2 is a concatenation of two 3-digit
 *          primes.
 * https://oeis.org/A153164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that p^2 is the concatenation of two 3‑digit primes */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Static state: list of qualifying primes */
static seq_t *matches = NULL;
static size_t matches_len = 0;
static size_t matches_cap = 0;

/* Build the list of primes up to 999 that satisfy the condition.
   This function is called lazily on the first invocation of a(). */
static void init(void) {
    if (matches != NULL) return; /* already initialized */

    /* Get all primes up to 999 */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(999, &primes, &cnt);

    /* Scan primes p with 100 <= p <= 999 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p < 100) continue;          /* ensure 3‑digit prime */

        uint64_t sq = p * p;             /* p^2 fits in 64‑bit */
        uint64_t high = sq / 1000;       /* first three digits */
        uint64_t low  = sq % 1000;       /* last three digits */

        if (high < 100 || low < 100) continue; /* must be three‑digit */

        if (is_prime((idx_t)high) && is_prime((idx_t)low)) {
            /* Grow array if necessary */
            if (matches_len == matches_cap) {
                size_t new_cap = matches_cap ? matches_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(matches, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed, abort safely */
                    free(primes);
                    abort();
                }
                matches = tmp;
                matches_cap = new_cap;
            }
            /* Store the qualifying prime */
            matches[matches_len++] = (seq_t)p;
        }
    }

    free(primes);
}

/* Oracle function for OEIS A153164 */
seq_t a(idx_t n) {
    /* lazy initialization */
    init();

    if ((size_t)n == 0 || (size_t)n > matches_len) {
        return 0;          /* no such prime for this index */
    }
    return matches[(size_t)n - 1];
}
