/*
 * A168285  a(n) = ((n-th nonprime)-th prime) - (n-th nonprime).
 * https://oeis.org/A168285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A018252(n)) - A018252(n)
 * Range:     u8 n <= 48, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 73 terms through n = 2374 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The A018252 sequence is the list of non‑prime numbers (starting with 1). */
/* For each n we find the n‑th nonprime x, then obtain the x‑th prime via */
/* the helper get_prime(k).  The difference is returned as seq_t. */

#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* Find the n‑th nonprime (A018252) */
    idx_t count = 0;
    idx_t x = 1;          /* nonprime list starts at 1 */
    idx_t np = 0;         /* value of the n‑th nonprime */

    while (1) {
        if (!is_prime(x)) {
            ++count;
            if (count == n) {
                np = x;
                break;
            }
        }
        ++x;
    }

    /* Obtain the np‑th prime using the provided helper */
    uint32_t prime = get_prime(np);

    /* Compute the required difference */
    seq_t result = (seq_t)prime - (seq_t)np;
    return result;
}
