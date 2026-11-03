/*
 * A136437  a(n) = prime(n) - k! where k is the greatest number such that k!
 *          <= prime(n).
 * https://oeis.org/A136437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - k! where k is the largest integer with k! <=
 *            prime(n)
 * Range:     u8 n <= 46, u16 n <= 4774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9519 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
    /* Precompute factorials up to 20! (sufficient for n up to many millions). */
    static seq_t fact[21];
    static bool fact_init = false;
    if (!fact_init) {
        fact[0] = 1;
        for (idx_t i = 1; i <= 20; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_init = true;
    }

    /* Get the n-th prime. */
    uint32_t p = get_prime(n);

    /* Determine the greatest k with k! <= p. */
    int k = 0;
    for (int i = 1; i <= 20; ++i) {
        if (fact[i] <= (seq_t)p)
            k = i;
        else
            break;
    }

    /* a(n) = prime(n) - k! . */
    return (seq_t)p - fact[k];
}
