/*
 * A233671  Numbers k such that prime(k)^2 < prime(k-1)*prime(k+1).
 * https://oeis.org/A233671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that prime(k)^2 <
 *            prime(k-1)*prime(k+1)
 * Range:     u8 n <= 25, u16 n <= 3467, u32 n <= 3467, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9380 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static idx_t current_k = 2;
    static seq_t pk_minus = 0, pk = 0, pk_next = 0;
    static idx_t *terms = NULL;
    static size_t terms_cap = 0, terms_len = 0;

    if (!initialized) {
        /* Initialize prime values for k = 1,2,3 */
        pk_minus = (seq_t)get_prime(1);  /* p_1 */
        pk       = (seq_t)get_prime(2);  /* p_2 */
        pk_next  = (seq_t)get_prime(3);  /* p_3 */
        current_k = 2;                   /* k starts from 2 */
        initialized = true;
    }

    /* Ensure we have at least 'n' terms */
    while (terms_len < (size_t)n) {
        /* Check inequality: prime(k)^2 < prime(k-1)*prime(k+1) */
        if (pk * pk < pk_minus * pk_next) {
            /* Grow the terms array if necessary */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                if (!tmp) abort();  /* Memory allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = current_k;
        }
        /* Advance to the next k */
        current_k++;
        pk_minus = pk;
        pk = pk_next;
        pk_next = (seq_t)next_prime((uint64_t)pk);  /* first prime > pk */
    }

    /* Return the requested term as seq_t */
    return (seq_t)terms[(size_t)(n - 1)];
}
