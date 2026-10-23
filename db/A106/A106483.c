/*
 * A106483  Primes p such that 2*p^2 - 1 is also prime.
 * https://oeis.org/A106483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth prime p such that 2*p^2-1 is also prime */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* static state for lazy construction */
    static seq_t *terms = NULL;
    static size_t cap   = 0, cnt  = 0;
    static uint64_t next = 2;       /* next integer to test for primality */

    if (n == 0) return 0;          /* behaviour below the offset is irrelevant */

    /* grow the list until it contains at least n terms */
    while (cnt < n) {
        /* find next prime */
        while (!is_prime_uint64(next)) {
            ++next;
        }
        uint64_t p = next;
        /* advance for the next search (skip even numbers after 2) */
        next = (p == 2 ? 3 : p + 2);

        /* compute q = 2*p^2-1 using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 q128 = (unsigned __int128)2 * (unsigned __int128)p * p - 1;
        if (q128 > UINT64_MAX) {          /* for practical n this never occurs */
            break;
        }
        uint64_t q = (uint64_t)q128;

        /* test the secondary prime condition */
        if (is_prime_uint64(q)) {
            /* ensure capacity */
            if (cnt >= cap) {
                size_t new_cap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;      /* allocation failure – fallback */
                terms = tmp;
                cap   = new_cap;
            }
            terms[cnt++] = (seq_t)p;
        }
    }

    return terms[n - 1];
}
