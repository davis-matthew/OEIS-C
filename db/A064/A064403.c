/*
 * A064403  Numbers k such that prime(k) + k and prime(k) - k are both primes.
 * https://oeis.org/A064403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 575, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 984 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064403
   Numbers k such that prime(k) + k and prime(k) - k are both primes.
   Implementation uses the provided get_prime(k) and isPrime(n) helpers
   to avoid rebuilding a sieve from scratch.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <stddef.h>
#include "../utils/is_prime.h"

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

/* State is maintained between calls for efficiency. */
static struct {
    uint32_t *terms;     /* array of k‑values already found */
    size_t  termCnt;     /* number of terms stored */
    size_t  termCap;     /* capacity of the array */
    uint32_t nextK;      /* next k to be tested */
} a_state = {0, 0, 0, 1};

/* Append a new term to the cache. */
static void append_term(uint32_t k)
{
    if (a_state.termCnt == a_state.termCap) {
        size_t newCap = a_state.termCap ? a_state.termCap * 2 : 1;
        uint32_t *tmp = (uint32_t*)realloc(a_state.terms, newCap * sizeof(uint32_t));
        a_state.terms = tmp;
        a_state.termCap = newCap;
    }
    a_state.terms[a_state.termCnt++] = k;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (a_state.termCnt < (size_t)n) {
        uint32_t k = a_state.nextK;
        uint32_t p = get_prime(k);                /* prime(k)          */
        uint32_t plus = p + k;
        uint32_t minus = p > k ? p - k : 0;

        bool ok_plus  = is_prime(plus);
        bool ok_minus = (minus > 1) && is_prime(minus);

        if (ok_plus && ok_minus)
            append_term(k);

        a_state.nextK++;
    }

    return (seq_t)a_state.terms[(size_t)n - 1];
}
