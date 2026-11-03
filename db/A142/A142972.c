/*
 * A142972  a(n) = the largest integer such that each positive integer <= a(n)
 *          divides at least one integer k, where the n-th prime <= k <= the
 *          (n+1)th prime.
 * https://oeis.org/A142972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest integer such that each positive integer <=
 *            a(n) divides at least one integer k in the closed interval [p_n,
 *            p_{n+1}], where p_n is the n-th prime. (OEIS A142972)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-01)
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

seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset; behaviour for n == 0 is irrelevant */
    idx_t p1 = (idx_t)get_prime(n);        /* nth prime */
    idx_t p2 = (idx_t)get_prime(n + 1);    /* (n+1)-th prime */

    /* All integers i <= L (the prime gap) automatically have a multiple
       in the interval, so we can start testing from i = L+1. */
    idx_t L = p2 - p1;                     /* prime gap */

    /* Search for the smallest i > L such that no multiple of i lies in
       [p1, p2].  The first such i gives a(n) = i - 1. */
    for (idx_t i = L + 1; ; ++i) {
        /* Count multiples of i in the interval. */
        idx_t cnt = p2 / i - (p1 - 1) / i;
        if (cnt == 0) {
            return (seq_t)(i - 1);
        }
    }
}
