/*
 * A083555  Quotient of LCM of prime(n+1)-1 and prime(n)-1 and GCD of the same
 *          two numbers.
 * https://oeis.org/A083555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(prime(n+1)-1, prime(n)-1) / gcd(prime(n+1)-1,
 *            prime(n)-1)
 * Range:     u8 n <= 8, u16 n <= 96, u32 n <= 9363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9471 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n-th term of OEIS A083555 */

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

seq_t a(idx_t n) {
    /* The offset of the sequence is 1, so n >= 1. */
    uint32_t p1 = get_prime(n);      /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);  /* prime(n+1) */

    seq_t x = (seq_t)p1 - (seq_t)1;  /* prime(n)   - 1 */
    seq_t y = (seq_t)p2 - (seq_t)1;  /* prime(n+1) - 1 */

    seq_t g   = gcd(x, y);          /* gcd of the two numbers */
    seq_t lcm = (x / g) * y;        /* lcm = x/g * y   (avoids overflow) */

    return lcm / g;                 /* result = lcm / gcd */
}
