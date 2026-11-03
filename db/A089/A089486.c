/*
 * A089486  The second-smallest prime of the form (p-prime(n))/(prime(n)-1),
 *          where p is also prime.
 * https://oeis.org/A089486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the second smallest prime r such that prime(n) +
 *            (prime(n) - 1) * r is prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 939 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* obtain the nth prime */
    seq_t q_seq = get_prime(n);
    uint64_t q = (uint64_t)q_seq;            /* fits in 64 bits for n ≤ 10⁴ */
    uint64_t d = q - 1;                      /* denominator in the ratio */

    uint64_t count = 0;
    seq_t found = 0;                          /* will hold the second r */

    /* iterate over all prime r (starting from 2) */
    for (uint64_t r = 2; ; ++r) {
        if (!is_prime(r)) continue;           /* skip non‑primes */

        /* p = q + d * r */
        unsigned __int128 prod = (unsigned __int128)d * r;
        uint64_t p = q + (uint64_t)prod;     /* product fits in 64 bits */

        if (is_prime(p)) {
            ++count;
            if (count == 2) {                /* second prime r found */
                found = (seq_t)r;
                break;
            }
        }
    }
    return found;
}
