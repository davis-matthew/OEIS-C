/*
 * A239374  Smallest product of consecutive distinct prime factors of t =
 *          prime(n)^2 - 1 in ascending order that provides more than 1/3
 *          factored parts for Brillhart-Lehmer-Selfridge primality test for
 *          prime(n).
 * https://oeis.org/A239374
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1650, u16 n <= 6622, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9550 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   a(n) = smallest product of consecutive distinct prime factors of  

   t = prime(n)^2 – 1 such that ∏(q_i^{e_i}) > prime(n)^{1/3}.  
   The function returns ∏ q_i (product of the distinct primes only).  
   Defined for n ≥ 2.  
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    /* the sequence is defined for n ≥ 2; for smaller n return 0 */
    if (n < 2) return 0;

    /* prime(n) */
    uint64_t p = (uint64_t)get_prime(n);          /* p fits in 64 bits */
    uint64_t t = p * p - 1ULL;                   /* t fits in 64 bits */

    /* factor t : collect distinct primes and their exponents */
    const size_t MAX_FACT = 32;
    uint64_t fac[MAX_FACT];
    uint32_t exp[MAX_FACT];
    size_t faccnt = 0;

    uint64_t u = t;
    idx_t k = 1;                                 /* primes are 1‑based */
    while (u > 1)
    {
        uint64_t prime = (uint64_t)get_prime(k);
        if (prime * prime > u) break;

        if (u % prime == 0)
        {
            uint32_t e = 0;
            while (u % prime == 0) { u /= prime; e++; }
            fac[ faccnt ]  = prime;
            exp[ faccnt ]  = e;
            faccnt++;
        }
        k++;
    }
    if (u > 1)          /* remaining prime factor */
    {
        fac[ faccnt ] = u;
        exp[ faccnt ] = 1;
        faccnt++;
    }

    /* search for the smallest prefix that gives product_exponent > p^{1/3} */
    uint64_t prod_exp   = 1;       /* product of primes^exponent so far */
    seq_t   prod_primes = (seq_t)1;/* product of distinct primes so far */

    for (size_t i = 0; i < faccnt; ++i)
    {
        uint64_t q = fac[i];
        uint32_t e = exp[i];

        /* compute q^e */
        uint64_t qpow = 1;
        for (uint32_t j = 0; j < e; ++j) qpow *= q;

        prod_exp   *= qpow;
        prod_primes *= (seq_t)q;

        /* check prod_exp^3 > p <=> prod_exp > p^{1/3} */
        unsigned __int128 pow3 = (unsigned __int128)prod_exp *
                                 (unsigned __int128)prod_exp *
                                 (unsigned __int128)prod_exp;
        if (pow3 > (unsigned __int128)p) return prod_primes;
    }
    /* if no prefix satisfies the condition, return the product of all primes */
    return prod_primes;
}
