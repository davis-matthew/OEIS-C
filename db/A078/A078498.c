/*
 * A078498  Let q(n) be the prime defined in A078497; sequence gives
 *          (q(n)-prime(n))/6.
 * https://oeis.org/A078498
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (min{p : p > prime(n), p and 2*prime(n)-p are primes} -
 *            prime(n)) / 6
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 96 terms through n = 100 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) equals the smallest k≥1 such that prime(n)–6k and prime(n)+6k are primes; return k. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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
#include <stdint.h>

seq_t a(idx_t n) {
    /*  The sequence is defined only for n ≥ 5.  Return 0 for smaller indices. */
    if (n < 5) return 0;

    /* Fetch the n-th prime.  get_prime returns a uint32_t; cast to idx_t. */
    idx_t pn = (idx_t)get_prime(n);

    idx_t k = 1;
    for (;;) {
        idx_t lower = pn - 6 * k;
        idx_t upper = pn + 6 * k;

        /* lower must be ≥ 2 to be a prime; for n ≥ 5 this is always true for the first few k, 
           but we guard against underflow. */
        if (lower >= 2 && is_prime(lower) && is_prime(upper))
            return (seq_t)k;

        ++k;
    }
}
