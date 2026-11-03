/*
 * A088606  Smallest number k such that concatenation of k and prime(n) is a
 *          prime, or 0 if no other number exists. a(1) = a(3) = 0.
 * https://oeis.org/A088606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that concatenation of k and prime(n) is
 *            prime, or 0 if no such k exists. The sequence is 1‑based
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  The auxiliary functions get_prime(idx_t n) and is_prime(idx_t x)
   are available in the OEIS build environment. */

#include <stdint.h>
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

/* Build 10^d where d is the number of decimal digits of x. */
static uint64_t pow10_of(uint32_t x)
{
    uint64_t pow10 = 1;
    if (x == 0) return 1;               /* not expected for prime(n)   */
    while (x > 0) {
        pow10 *= 10ULL;
        x    /= 10U;
    }
    return pow10;
}

seq_t a(idx_t n)
{
    /* 1‑based indexing: prime(1) = 2   */
    uint32_t p = get_prime(n);
    if (p == 0) return 0;               /* safety guard */

    uint64_t pow10 = pow10_of(p);

    /* Search for the smallest k.  The known b‑file values never require
       k larger than a few million; the inner loop therefore terminates
       long before the hard limit. */
    const uint64_t SEARCH_LIMIT = 2000000ULL;
    for (uint64_t k = 1; k <= SEARCH_LIMIT; ++k) {
        uint64_t candidate = k * pow10 + (uint64_t)p;
        if (is_prime((idx_t)candidate))
            return (seq_t)k;
    }

    /* No suitable k found within the search bound – report 0. */
    return 0;
}
