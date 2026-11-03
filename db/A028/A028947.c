/*
 * A028947  Difference between P(n) and nearest integer to P(n-1)^2 / P(n-2),
 *          where P(i) =i-th prime.
 * https://oeis.org/A028947
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = P(n) - nearestint(P(n-1)^2 / P(n-2)) (n ≥ 3)
 * Range:     i8 n <= 32, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nearestint(x) is computed as floor(x + ½).                     */
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
    if (n < 3)                     /* indices below the offset are undefined */
        return 0;

    seq_t pn  = (seq_t)get_prime(n);
    seq_t pn1 = (seq_t)get_prime(n - 1);
    seq_t pn2 = (seq_t)get_prime(n - 2);

    /* round(Pn1^2 / Pn2) as floor((Pn1^2 + Pn2/2)/Pn2)   */
    seq_t rounded = (pn1 * pn1 + pn2 / 2) / pn2;

    return pn - rounded;
}
