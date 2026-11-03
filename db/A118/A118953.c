/*
 * A118953  Number of ways to write the n-th prime as 2^k + p, where p is
 *          prime and p < 2^k.
 * https://oeis.org/A118953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of the n-th prime as 2^k + p
 *            with p prime and p < 2^k. The count is 0 or 1 for all n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of OEIS range, safe return */

    uint32_t prime_n  = get_prime(n);      /* the n-th prime, n ≥ 1 */
    seq_t    count    = 0;
    uint64_t pow2     = 1;                 /* 2^k */

    /* iterate while 2^k < prime_n */
    while (pow2 < (uint64_t)prime_n) {
        uint32_t p_sub = prime_n - (uint32_t)pow2;    /* candidate p' = prime_n - 2^k */

        /* p' must be prime and < 2^k */
        if (p_sub < (uint32_t)pow2 && is_prime((idx_t)p_sub))
            ++count;          /* at most one representation, but count generically */

        pow2 <<= 1;          /* next power of two */
    }

    return count;
}
