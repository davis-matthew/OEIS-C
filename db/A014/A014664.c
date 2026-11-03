/*
 * A014664  Order of 2 modulo the n-th prime.
 * https://oeis.org/A014664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 81 terms through n = 9467 (2026-11-01)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A014664:
   a(n) = multiplicative order of 2 modulo the n-th prime
   (offset 2; a(2) = 2, a(3) = 4, ...) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"

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
    /* For indices < 2 the sequence is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* n-th prime, using the provided UII. */
    uint32_t p = get_prime(n);           /* prime(p_n) */
    uint64_t phi = (uint64_t)p - 1ULL;   /* p - 1, divides the order */

    /* Factorize phi. 64‑bit primes are sufficient for tests up to p≈10⁶. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi, primes, exps, &cnt);

    /* Compute the multiplicative order of 2 modulo p. */
    seq_t order = (seq_t)phi;               /* start with max possible order */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)primes[i];
        /* Try reducing the order by successive powers of the prime factor. */
        while ((order % q) == (seq_t)0) {
            seq_t next = order / q;
            /* Check if 2^(next) ≡ 1 (mod p). */
            seq_t res = powmod((seq_t)2, (uint64_t)next, (seq_t)p);
            if (res == (seq_t)1)
                order = next;              /* reduction succeeded */
            else
                break;                     /* cannot reduce further by this factor */
        }
    }

    return order;
}
