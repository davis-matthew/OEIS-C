/*
 * A125138  a(n) = smallest m such that prime(n) divides Sum_{i=1..m} i!, or
 *          -1 if no such m exists.
 * https://oeis.org/A125138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest m such that prime(n) divides sum_{i=1..m} i!, or
 *            -1 if no such m exists
 * Range:     i8 n <= 33, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the utility that returns the k‑th prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑based). */
    uint32_t p = get_prime(n);   /* p fits in 32 bits for n up to 10 000 */

    /* Work modulo p using 64‑bit arithmetic to avoid overflow. */
    uint64_t P = p;
    seq_t sum  = 0;      /* current Σ i!  (mod p) */
    seq_t fact = 1;      /* current i!   (mod p) */

    /* Check values of m < p; i ranges from 1 up to p-1. */
    for (idx_t i = 1; i < p; ++i)
    {
        /* fact = fact * i (mod p) */
        uint64_t prod = (uint64_t)fact * (uint64_t)i;
        fact = (seq_t)(prod % P);

        /* sum += fact  (mod p) */
        uint64_t s = (uint64_t)sum + (uint64_t)fact;
        sum = (seq_t)(s % P);

        if (sum == (seq_t)0)   /* first m with Σ i! ≡ 0 (mod p) */
            return (seq_t)i;
    }
    /* No m < p satisfies the condition */
    return (seq_t)-1;
}
