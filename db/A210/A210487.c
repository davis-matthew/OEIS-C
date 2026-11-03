/*
 * A210487  a(n) is the smallest possible greatest prime factor of prime(n)^2
 *          - prime(k)^2 for 0 < k < n.
 * https://oeis.org/A210487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimum_{1 <= k < n} GPF(p_n^2 - p_k^2) Computed as
 *            min_{k} max( GPF(p_n - p_k), GPF(p_n + p_k) )
 * Range:     u8 n <= 55, u16 n <= 9019, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9993 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/largest_prime_factor.h"

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
    if (n < 2)
        return (seq_t)0;               /* undefined for n < 2 */

    uint32_t p_n = get_prime(n);        /* prime(n) */
    seq_t best = (seq_t)ULLONG_MAX;    /* large initial value */

    for (idx_t k = 1; k < n; ++k)
    {
        uint32_t p_k = get_prime(k);

        uint64_t d1 = (uint64_t)p_n - (uint64_t)p_k; /* p_n - p_k */
        uint64_t d2 = (uint64_t)p_n + (uint64_t)p_k; /* p_n + p_k */

        uint64_t g1 = largest_prime_factor(d1);
        uint64_t g2 = largest_prime_factor(d2);
        uint64_t g  = g1 > g2 ? g1 : g2;           /* GPF of the difference */

        if ((seq_t)g < best)
        {
            best = (seq_t)g;
            if (best == (seq_t)2)   /* 2 is the smallest possible GPF */
                break;
        }
    }

    return best;
}
