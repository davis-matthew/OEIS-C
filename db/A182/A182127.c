/*
 * A182127  Main diagonal T(n,n) of array T(k,n) = n-th value of m^prime(k) +
 *          m*prime(k) + prime(k)^m.
 * https://oeis.org/A182127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^p_n + (n-1)*p_n + p_n^(n-1) where p_n is the nth
 *            prime and all operations are in seq_t
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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
    /* Convert index to seq_t when needed */
    idx_t m_idx = n - 1;                     /* m = n-1 */
    uint32_t p_uint = get_prime(n);          /* p_n as uint32_t */

    seq_t m = (seq_t)m_idx;                  /* (n-1) as seq_t */
    seq_t p = (seq_t)p_uint;                 /* nth prime as seq_t */

    /* Compute the three terms */
    seq_t term1 = ipow(m, p);             /* (n-1)^p_n   */
    seq_t term2 = m * p;                     /* (n-1)*p_n   */
    seq_t term3 = ipow(p, m);             /* p_n^(n-1)   */

    /* Sum them to get a(n) */
    return term1 + term2 + term3;
}
