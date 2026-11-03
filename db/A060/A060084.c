/*
 * A060084  a(n) is the least prime not a primitive root of n-th prime.
 * https://oeis.org/A060084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9757 (2026-11-02)
 * Helpers:   factorize, is_prime_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A060084: a(n) = the least prime that is NOT a primitive root
 * of the n‑th prime (index 1‑based). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/is_prime_seq.h"
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
    /* The n‑th prime (1‑based). */
    uint32_t q_u32 = get_prime(n);
    uint64_t q = (uint64_t)q_u32;              /* promote for arithmetic */

    /* Handle the trivial base case q = 2 directly. */
    if (q == 2)
        return (seq_t)2;

    /* Factor q-1. */
    uint64_t fac[64];          /* prime factors of q-1 */
    int      exp[64];          /* their exponents (unused) */
    size_t   cnt = factorize(q - 1, fac, exp);

    /* Search the least prime which is not a primitive root modulo q. */
    for (uint64_t p = 2; ; ++p) {
        if (!is_prime_seq(p))
            continue;              /* skip composites */

        /* If p == q (i.e., p mod q == 0), it cannot be a primitive root. */
        if (p % q == 0)
            return (seq_t)p;

        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            uint64_t exp_val = (q - 1) / fac[i];
            seq_t   v = powmod((seq_t)p, exp_val, (seq_t)q);
            if (v == (seq_t)1)           /* violates primitive root test */
                ok = false;
        }
        if (!ok)
            return (seq_t)p;
    }
}
