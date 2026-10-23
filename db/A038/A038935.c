/*
 * A038935  Primes p such that 57 is a square mod p.
 * https://oeis.org/A038935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, jacobi_symbol, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence: primes p for which 57 is a quadratic residue modulo p
 * (including p = 2).  The function searches primes incrementally and
 * caches the result so that calls are independent of order. */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/seq_realloc.h"    /* or replace with appropriate header */

seq_t a(idx_t n) {
    /* static cache so that calls in any order stay independent */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t cand = 2;   /* next integer to test, starts at 2 */

    if (n == 0) return 0;    /* offset 1 – behaviour below offset doesn't matter */

    /* extend the cache until it contains at least n terms        */
    while (len < n) {
        /* find next prime that satisfies the condition           */
        while (1) {
            if (isPrimeGeneric((seq_t)cand)) {
                /* 2 is included automatically; for any other prime
                 * include it if 57 is a quadratic residue (Legendre=1)
                 * or if the prime divides 57 (Jacobi=0).            */
                if (cand == 2 || jacobi_symbol(57, (uint64_t)cand) != -1)
                    break;
            }
            cand++;
        }

        /* enlarge cache if necessary */
        if (len == cap) {
            size_t newcap = cap ? cap << 1 : 16;
            seq_t *tmp = seq_realloc(cache, cap, newcap); /* seq_realloc handles any seq_t type */
            assert(tmp != NULL);
            cache = tmp;
            cap = newcap;
        }

        cache[len++] = (seq_t)cand;
        cand++;                                   /* next candidate for next iteration */
    }

    return cache[n - 1];
}
