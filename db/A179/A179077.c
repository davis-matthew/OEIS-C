/*
 * A179077  a(n) is the residue ((2^p - 2)/p) mod p, where p is the n-th
 *          prime.
 * https://oeis.org/A179077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^p - 2)/p) mod p, where p is the n-th prime
 * Range:     u8 n <= 55, u16 n <= 6675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9528 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A179077 with arbitrary-precision seq_t
 * and 64-bit intermediate arithmetic. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/powmod_u64.h"

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
    if (n < 1) return (seq_t)0;                /* offset starts at 1 */

    /* nth prime (fits in uint32_t for the required range) */
    uint32_t p32 = get_prime(n);              /* p < 2^32 */
    uint64_t p  = (uint64_t)p32;

    /* Compute 2^p modulo p^2.  p^2 < 2^64 for p up to the 10,000th prime. */
    uint64_t pp = p * p;
    uint64_t r  = powmod_u64(2ULL, (uint64_t)p, pp);  /* 2^p mod p^2 */

    /* (r - 2) may underflow if r < 2; correct for that by adding p^2. */
    uint64_t diff;
    if (r >= 2ULL) {
        diff = r - 2ULL;
    } else {
        diff = r + pp - 2ULL;   /* r + p^2 - 2 is the non‑negative remainder */
    }

    /* quotient is < p, fits in seq_t. */
    uint64_t quotient = diff / p;
    return (seq_t)quotient;
}
