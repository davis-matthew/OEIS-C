/*
 * A226295  Multiplicative order of the n-th prime modulo the (n+1)th prime.
 * https://oeis.org/A226295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of the * n-th prime modulo the
 *            (n+1)-th prime
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9986 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
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
    /* Get the n-th and (n+1)-th primes */
    seq_t p = (seq_t)get_prime(n);          /* p_n */
    seq_t q = (seq_t)get_prime(n + 1);      /* p_{n+1} */

    seq_t m = q - 1;           /* φ(p_{n+1}) = p_{n+1} - 1  */
    seq_t order = m;           /* initial candidate for the order */

    /* Prime factorization of m (m <= 200000 for n <= 10000) */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    if (!prime_list) {
        /* Build a small list of primes for factorization; 200000 is safely
           above the square‑root of any m we will encounter. */
        build_primes_upto(200000, &prime_list, &prime_cnt);
    }

    seq_t temp = m;

    /* For each prime divisor p of m, try to reduce the candidate order. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t pr = (seq_t)prime_list[i];
        if (pr * pr > temp)  break;
        if (temp % pr == 0) {
            /* Remove all powers of pr from temp */
            while (temp % pr == 0) temp /= pr;

            /* Reduce order by pr as much as possible */
            while (order % pr == 0 &&
                   powmod(p, (uint64_t)(order / pr), q) == 1)
            {
                order /= pr;
            }
        }
    }

    /* If a prime factor larger than sqrt(m) remains */
    if (temp > 1) {
        seq_t pr = temp;  /* the remaining prime factor */
        while (order % pr == 0 &&
               powmod(p, (uint64_t)(order / pr), q) == 1)
        {
            order /= pr;
        }
    }

    return order;
}
