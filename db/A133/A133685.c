/*
 * A133685  Let p = prime(n); then a(n) = (sum of prime factors of p+1) - (sum
 *          of prime factors of p-1). a(1) = 2 by convention.
 * https://oeis.org/A133685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A001414(prime(n)+1) - A001414(prime(n)-1), a(1)=2
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 954 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

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

/* Sum the prime factors of m (with multiplicity). */
static seq_t sum_prime_factors_uint64(uint64_t m)
{
    if (m < 2) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    seq_t sum = 0;
    factor_u64(m, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    uint32_t p = get_prime(n);        /* n-th prime */
    uint64_t m1 = (uint64_t)p + 1;    /* prime + 1 */
    uint64_t m2 = (uint64_t)p - 1;    /* prime - 1 */
    seq_t sum1 = sum_prime_factors_uint64(m1);
    seq_t sum2 = sum_prime_factors_uint64(m2);
    return sum1 - sum2;
}
