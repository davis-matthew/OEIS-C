/*
 * A143244  a(n) = the smallest multiple of the n-th prime such that (a(n)-1)
 *          is divisible by both the (n-1)th prime and the (n+1)st prime.
 * https://oeis.org/A143244
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal multiple of p_n such that a(n)-1 ≡ 0 (mod
 *            p_{n-1}·p_{n+1})
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 34 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as: let L = p_{n-1}*p_{n+1}, find inverse inv of p_n mod L, then a(n) = inv*p_n */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
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
    if (n < 2) return (seq_t)0; /* Undefined for n < 2, return 0 */

    /* primes surrounding the n-th prime */
    uint32_t p_prev = get_prime(n-1); /* (n-1)-th prime */
    uint32_t p_curr = get_prime(n);   /* n-th prime     */
    uint32_t p_next = get_prime(n+1); /* (n+1)-th prime */

    /* L = p_{n-1} * p_{n+1} */
    seq_t L = (seq_t)p_prev * (seq_t)p_next;

    /* Euler phi of L = (p_prev-1)*(p_next-1) */
    seq_t phiL = (seq_t)(p_prev-1) * (seq_t)(p_next-1);

    /* exponent for modular inverse: phi(L)-1 */
    seq_t exp_seq = phiL - (seq_t)1;
    idx_t exp = (idx_t)exp_seq; /* fits in 64 bits for n ≤ 10000 */

    /* Inverse of p_curr modulo L: p_curr^{phi(L)-1} mod L */
    seq_t inv = powmod((seq_t)p_curr, exp, L);

    /* Result */
    seq_t result = inv * (seq_t)p_curr;
    return result;
}
