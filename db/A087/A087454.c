/*
 * A087454  Multiplicative inverse of the n-th prime prime(n) modulo
 *          prime(n-1).
 * https://oeis.org/A087454
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = inverse_mod( prime(n), prime(n-1) ) (i.e., the
 *            multiplicative inverse of the n-th prime modulo the (n-1)-st
 *            prime)
 * Range:     u8 n <= 54, u16 n <= 6795, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9882 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers from the utilities */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/powmod.h"

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

seq_t a(idx_t n)
{
    if (n < 2)                /* a(2) is the first defined term; below that we may return any value. */
        return (seq_t)0;

    /* Retrieve the consecutive primes. */
    seq_t p_prev = get_prime(n - 1);   /* prime(n‑1)   */
    seq_t p_curr = get_prime(n);       /* prime(n)     */

    /* Compute the inverse via Fermat's little theorem:
       p_curr^(p_prev-2) mod p_prev  */
    uint64_t exp = (uint64_t)(p_prev - (seq_t)2);
    return powmod(p_curr, exp, p_prev);
}
