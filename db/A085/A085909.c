/*
 * A085909  Smallest prime p>prime(n) such that p+prime(n+1)-prime(n) is the
 *          next prime after p; or a(n)=0 if no such prime exists.
 * https://oeis.org/A085909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime p > prime(n) such that p + prime(n+1)
 *            - prime(n) is the next prime after p. If no such prime exists,
 *            a(n) = 0. (For n = 1 this is defined as 0.)
 * Range:     u8 n <= 23, u16 n <= 3076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8946 (2026-11-01)
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
    /* 1‑based offset */
    if (n == 1) return (seq_t)0;

    /* n‑th prime and (n+1)‑th prime */
    uint32_t p_n   = get_prime(n);     /* prime(n)   */
    uint32_t p_np1 = get_prime(n + 1); /* prime(n+1) */

    /* Desired gap between consecutive primes */
    uint32_t diff = p_np1 - p_n;

    /* Search primes p > prime(n) having gap = diff */
    uint64_t p = next_prime((uint64_t)p_n + 1);   /* first prime > prime(n) */

    for (;;)
    {
        uint64_t q = next_prime(p + 1);          /* next prime after p */
        if ((uint32_t)(q - p) == diff)          /* gap matches diff?  */
            return (seq_t)p;                     /* smallest such p found */
        p = q;                                   /* continue searching */
    }
}
