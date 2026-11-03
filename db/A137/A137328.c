/*
 * A137328  a(n) = prime(n) - primorial(k), where k is the greatest number for
 *          which primorial(k) <= prime(n).
 * https://oeis.org/A137328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - primorial(k), where k = the greatest integer
 *            such that primorial(k) <= prime(n)
 * Range:     u8 n <= 54, u16 n <= 7607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9108 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  Sequence A137328 – difference between the nth prime and the largest
    primorial not exceeding it.  Computes directly without pre‑saving
    tables, works for any supported seq_t type and any idx_t input.     */
/* --------------------------------------------------------------------- */

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
    /* The sequence is defined only for n >= 1. */
    if (n < 1) return 0;

    /* Prime at position n. */
    seq_t prime_n = (seq_t)get_prime(n);

    /* Compute primorial(k) iteratively. */
    seq_t primorial = 1;        /* primorial(0) */
    idx_t  k = 0;               /* current exponent */

    for (;;) {
        idx_t next_k = k + 1;
        seq_t next_prime = (seq_t)get_prime(next_k);

        /* If multiplying would exceed prime_n, stop.
           Avoid overflow by using division instead of multiplication. */
        if (prime_n / primorial < next_prime) {
            break;                 /* primorial(k) is now the largest <= prime_n */
        }

        primorial = primorial * next_prime;
        k = next_k;
    }

    /* Return the difference. */
    return prime_n - primorial;
}
