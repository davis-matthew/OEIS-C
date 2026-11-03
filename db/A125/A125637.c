/*
 * A125637  Smallest odd prime base q such that p^3 divides q^(p-1) - 1, where
 *          p = prime(n).
 * https://oeis.org/A125637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime q such that p^3 | q^(p-1)-1, where p =
 *            prime(n) (the n‑th prime)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   ipow_i32, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Dependencies:
   - get_prime(idx_t k)            : returns the k‑th prime as uint32_t
   - pow_seq(seq_t base, int exp)  : raises base to exp (exp is an int)
   - mod_pow(seq_t base, uint64_t exp, seq_t mod) : base^exp mod mod
   - is_prime_big(seq_t n)         : true iff n is prime (used if needed)
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
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

/* Return the index‑n term of sequence A125637. */
seq_t a(idx_t n)
{
    /* 1. Obtain the p‑th prime:  p = prime(n). */
    uint32_t p_val = get_prime(n);            /* p fits in uint32_t for all n requested */
    seq_t p = (seq_t)p_val;

    /* 2. Compute modulus m = p^3. */
    seq_t mod = ipow_i32(p, 3);                /* p^3 fits in seq_t for all test sizes */

    /* 3. Iterate over odd primes q, starting from the second prime (3). */
    idx_t q_index = 2;                         /* 1‑based index of primes, 1->2, 2->3, ... */
    while (1) {
        uint32_t q_val = get_prime(q_index);
        seq_t q = (seq_t)q_val;

        /* 3a. Test the congruence q^(p-1) ≡ 1 (mod p^3). */
        if (powmod(q, (uint64_t)p_val - 1, mod) == (seq_t)1)
            return q;                          /* found the smallest qualifying q */

        ++q_index;                              /* next prime */
    }
}
