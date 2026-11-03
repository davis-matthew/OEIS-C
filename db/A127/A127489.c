/*
 * A127489  a(n) is the coefficient of the linear term in the polynomial
 *          (x-prime(n))*(x-prime(n+1))*(x-prime(n+2))*(x-prime(n+3))*(x-prime(n+4)).
 * https://oeis.org/A127489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = arithmetic derivative of the product of five consecutive
 *            primes. * That is, a(n) = Σ_{i=0..4} (P_n P_{n+1} … P_{n+4} /
 *            P_{n+i}), * where P_k is the k-th prime
 * Range:     u8 none, u16 n <= 2, u32 n <= 21, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Get the five consecutive primes. */
    seq_t primes[5];
    for (idx_t i = 0; i < 5; ++i)
        primes[i] = (seq_t) get_prime(n + i);

    /* Compute the product of the five primes. */
    seq_t prod = 1;
    for (idx_t i = 0; i < 5; ++i)
        prod *= primes[i];

    /* Compute the sum of product divided by each prime. */
    seq_t sum = 0;
    for (idx_t i = 0; i < 5; ++i)
        sum += prod / primes[i];

    return sum;
}
