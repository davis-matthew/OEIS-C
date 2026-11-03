/*
 * A140893  a(n) = prime(n)^prime(n+1) - prime(n+1)^prime(n).
 * https://oeis.org/A140893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n)^prime(n+1) - prime(n+1)^prime(n)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 70 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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
    /* OEIS offset: a(1) is first term */
    if (n < 1) {
        return (seq_t)0; /* undefined for n < 1 */
    }
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);
    seq_t prime_n   = (seq_t)p_n;
    seq_t prime_np1 = (seq_t)p_np1;

    /* big_pow handles large exponents; works for both arbitrary-precision
       and fixed-width types. */
    seq_t pow1 = ipow(prime_n,   prime_np1); /* prime(n)^prime(n+1) */
    seq_t pow2 = ipow(prime_np1, prime_n);   /* prime(n+1)^prime(n)   */

    return pow1 - pow2;
}
