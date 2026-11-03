/*
 * A232106  Number of groups of order prime(n)^6.
 * https://oeis.org/A232106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of groups of order prime(n)^6. For a prime p > 3:
 *            a(n) = 3p² + 39p + 344 + 24 gcd(p-1,3) + 11 gcd(p-1,4) + 2
 *            gcd(p-1,5). Special cases: a(1)=267 (for p=2), a(2)=504 (for
 *            p=3)
 * Range:     u8 none, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 1000 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    /* Handle the offset and special cases for p = 2 and 3 */
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */
    if (n == 1) return (seq_t)267;  /* p = 2 */
    if (n == 2) return (seq_t)504;  /* p = 3 */

    /* Retrieve the nth prime */
    seq_t p = (seq_t)get_prime(n); /* get_prime returns uint32_t */

    /* Compute the formula for p > 3 */
    seq_t p2 = p * p;
    seq_t res = 3 * p2 + 39 * p + 344;

    /* GCD terms with small constants 3, 4, 5 */
    seq_t d3 = gcd(p - (seq_t)1, (seq_t)3);
    seq_t d4 = gcd(p - (seq_t)1, (seq_t)4);
    seq_t d5 = gcd(p - (seq_t)1, (seq_t)5);

    res += 24 * d3 + 11 * d4 + 2 * d5;
    return res;
}
