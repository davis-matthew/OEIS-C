/*
 * A139644  Primes of the form x^2 + 105*y^2.
 * https://oeis.org/A139644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139644: primes of the form x^2 + 105*y^2 */
/* The function generates primes that satisfy p ≡ 1,109,121,169,289,361 (mod 420). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    /* Residues allowed modulo 420. */
    static const idx_t allowed[6] = {1,109,121,169,289,361};

    /* Cached list of primes that satisfy the quadratic form. */
    static seq_t *special = NULL;
    static size_t spec_cnt = 0, spec_cap = 0;

    /* Next integer to test for primality. */
    static idx_t current = 2;

    /* Lazy allocation of the array. */
    if (!spec_cap) {
        spec_cap = 16;
        special = (seq_t *)malloc(spec_cap * sizeof(seq_t));
    }

    /* Generate filling the cache until we have at least n values. */
    while (spec_cnt < n) {
        /* Test current for primality. */
        if (is_prime(current)) {
            /* Check residue class modulo 420. */
            idx_t r = current % 420;
            for (int k = 0; k < 6; ++k) {
                if (r == allowed[k]) {
                    /* Grow array if necessary. */
                    if (spec_cnt == spec_cap) {
                        size_t new_cap = spec_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(special,
                                                      new_cap * sizeof(seq_t));
                        special = tmp;
                        spec_cap = new_cap;
                    }
                    special[spec_cnt++] = (seq_t)current;
                    break;
                }
            }
        }
        ++current;
    }

    return special[n - 1];
}
