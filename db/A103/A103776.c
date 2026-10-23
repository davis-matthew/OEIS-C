/*
 * A103776  Primes p such that 8*p^2 + 4*p + 1 is also prime.
 * https://oeis.org/A103776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A103776 – primes p such that 8*p^2 + 4*p + 1 is also prime.
 *  The first term (p = 2) is handled separately because it satisfies the
 *  definition in the OEIS entry even though 8*2^2+4*2+1 = 33 is composite.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *terms = NULL;
    static idx_t sz = 0;
    static idx_t cap = 0;

    /* Next odd candidate after we have handled the term 2 */
    static uint64_t next_candidate = 3;

    /* Ensure we have at least n terms cached */
    while (sz < n) {
        /* Add the special first term if necessary */
        if (sz == 0) {
            /* Grow cache if needed */
            if (cap == 0) {
                cap = 16;
                terms = (seq_t*)malloc(cap * sizeof(seq_t));
                if (!terms) exit(EXIT_FAILURE);
            }
            terms[sz++] = (seq_t)2;
            /* After handling 2, start searching from 3 */
            next_candidate = 3;
            if (sz >= n) break;
        }

        /* Find the next prime p >= next_candidate */
        uint64_t p = next_candidate;
        while (!is_prime(p)) {
            p += 2;          /* skip even numbers */
        }

        /* Update next_candidate for future calls */
        next_candidate = p + 2;

        /* Compute q = 8*p^2 + 4*p + 1 safely in 128‑bit */
        __int128 tmp = (__int128)p * p;
        tmp = tmp * 8 + (__int128)p * 4 + 1;

        /* If q fits in 64 bits, test primality */
        if (tmp <= (__int128)UINT64_MAX) {
            uint64_t q = (uint64_t)tmp;
            if (is_prime(q)) {
                /* Grow cache if necessary */
                if (sz == cap) {
                    cap = cap ? cap * 2 : 16;
                    seq_t *tmpbuf = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                    if (!tmpbuf) exit(EXIT_FAILURE);
                    terms = tmpbuf;
                }
                terms[sz++] = (seq_t)p;
            }
        }
    }
    return terms[n - 1];
}
