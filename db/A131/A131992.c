/*
 * A131992  a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4.
 * https://oeis.org/A131992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 43 terms through n = 939 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    /* OEIS offset: a(1) is the first term. */
    if (n == 0) return 0;                 /* undefined for n < 1 – handled gracefully */
    uint32_t pk = get_prime(n);            /* k‑th prime (k ≥ 1) */
    seq_t p   = (seq_t)pk;                 /* cast to seq_t type */
    seq_t res = 1;                         /* start with the leading 1 */
    seq_t term = p;                        /* current power of p */

    for (int i = 0; i < 4; ++i) {
        res += term;                       /* add p^i  (i = 0..3) */
        /* multiply using a 128‑bit intermediate to avoid overflow in uint64_t builds */
        __uint128_t mult = ( __uint128_t )term * ( __uint128_t )p;
        term = (seq_t)mult;                /* promote back to seq_t */
    }
    return res;
}
