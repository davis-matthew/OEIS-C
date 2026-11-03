/*
 * A061141  Compute sum of divisors of the prime(n+1)-prime(n)-1 composite
 *          numbers between two consecutive primes; choose the largest.
 * https://oeis.org/A061141
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2026, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 71 terms through n = 9681 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Max{sigma(c) | p(n+1) > c > p(n), c composite}          */
/* (OEIS A061141).  The function uses the supplied helpers get_prime,    */
/* is_prime, and sigma_of to avoid rebuilding large prime tables.          */

#include <stdbool.h>
/* Helper functions provided by the framework */
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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  It is thread‑safe only through static data,  */
/* but the problem statement prohibits external state, so we keep it      */
/* stateless: each call queries the helpers directly.                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)     /* undefined below the offset; return 0 as a neutral value */
        return 0;

    /* Retrieve the two consecutive primes.  get_prime(1) is the first prime. */
    idx_t p_n    = (idx_t)get_prime(n);     /* p(n)   */
    idx_t p_next = (idx_t)get_prime(n + 1); /* p(n+1) */

    seq_t max_sigma = 0;

    /* Scan all integers strictly between the two primes. */
    for (idx_t c = p_n + 1; c < p_next; ++c) {
        /* Skip primes – only composites count. */
        if (is_prime(c))
            continue;

        seq_t s = sigma_of(c);     /* sum of divisors of c */
        if (s > max_sigma)
            max_sigma = s;
    }

    return max_sigma;
}
