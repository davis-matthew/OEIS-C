/*
 * A230481  a(n) = prime(prime(2*n)) - prime(2*prime(n)).
 * https://oeis.org/A230481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(prime(2*n)) - prime(2*prime(n))
 * Range:     i8 n <= 13, i16 n <= 1624, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9644 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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
    if (n < 1U)
        return (seq_t)0;

    /* 2*n is always >= 2, so safe */
    idx_t two_n = 2 * n;
    idx_t p2n   = (idx_t)get_prime(two_n);            /* prime(2*n) */

    /* prime(p2n) */
    idx_t primed_p2n = (idx_t)get_prime(p2n);

    /* prime(n) */
    idx_t pn = (idx_t)get_prime(n);

    /* prime(2*pn) */
    idx_t two_pn = 2 * pn;
    idx_t prime_2pn = (idx_t)get_prime(two_pn);

    /* compute signed difference */
    long long diff = (long long)primed_p2n - (long long)prime_2pn;
    return (seq_t)diff;
}
