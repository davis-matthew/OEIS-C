/*
 * A051614  4th term in Euclid-Mullin prime sequence started with n-th prime
 *          (cf. A000945).
 * https://oeis.org/A051614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 953 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A051614: 4th term in the Euclid-Mullin prime sequence
 * starting with the n-th prime.
 *
 * For a starting prime p:
 *   a1 = p
 *   a2 = smallest prime factor of (a1 + 1)   (2 if p > 2, otherwise 3)
 *   a3 = smallest prime factor of (a1·a2 + 1) (2p+1 for p > 2)
 *   a4 = smallest prime factor of (a1·a2·a3 + 1)
 *
 * The function below computes a4 for the n-th prime (n ≥ 1).  
 * It builds a list of primes up to 1,000,000 once and reuses it.
 */

/* Include helper declarations */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

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

/* Helper: smallest prime factor of a number (returns the number itself
 * if it is prime).  Works for values that fit in seq_t (e.g. 64‑bit). */
static seq_t spf_for(seq_t n)
{
    /* primes_u64 is initialized by init_primes() */
    static uint64_t *primes_u64 = NULL;
    static size_t primes_cnt = 0;
    /* build primes on first use */
    if (!primes_u64) {
        /* 1,000,000 is safely larger than sqrt of the largest product
           we ever test (≈4.4 × 10¹⁰). */
        build_primes_upto(1000000, &primes_u64, &primes_cnt);
    }

    /* iterate over pre‑computed primes up to sqrt(n) */
    size_t i = 0;
    while (i < primes_cnt) {
        seq_t p = (seq_t)primes_u64[i];
        if (p * p > n) break;
        if ((n % p) == 0) return p;
        ++i;
    }
    /* n is prime */
    return n;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the OEIS specification. */
    uint32_t p32 = get_prime(n);          /* nth prime, fits in 32 bits */
    seq_t p = (seq_t)p32;

    seq_t b2, b3, result;

    if (p32 == 2) {
        b2 = 3;                           /* a2 for starting prime 2 */
        seq_t tmp = p * b2 + 1;           /* a1·a2 + 1 */
        b3 = spf_for(tmp);                /* a3 */
    } else {
        b2 = 2;                           /* a2 for odd primes */
        seq_t tmp = p * b2 + 1;           /* 2p + 1 */
        b3 = spf_for(tmp);                /* a3 */
    }

    seq_t prod = p * b2 * b3 + 1;          /* product of first 3 terms + 1 */
    result = spf_for(prod);                /* a4, the desired term */

    return result;
}
