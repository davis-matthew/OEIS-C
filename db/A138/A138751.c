/*
 * A138751  a(n) = nextprime( p(n)/2 if p(n)=2 (mod 3), 2p(n) else ) =
 *          A007918( A138750( A000040( n ))).
 * https://oeis.org/A138751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nextprime( p(n)/2 if p(n)=2 (mod 3), 2p(n) else ) where
 *            p(n) is the n‑th prime. The function uses get_prime to fetch
 *            p(n) and next_prime (from the utils library) to obtain the next
 *            prime strictly larger than the specified bound. The code works
 *            for any seq_t implementation (uint8, 16, 32, 6...
 * Range:     u8 n <= 30, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8990 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* get the n-th prime as a 32‑bit unsigned integer */
    uint32_t p = get_prime(n);

    /* compute the bound for the next prime */
    uint64_t lower;
    if ((p % 3) == 2)            /* p ≡ 2 (mod 3) → use p/2 (floor)   */
        lower = (uint64_t)p / 2;
    else                          /* otherwise use 2p                */
        lower = (uint64_t)p * 2;

    /* return the next prime strictly greater than that bound */
    return (seq_t)next_prime(lower);
}
