/*
 * A214714  Least m>0 such that prime(n)-m and n-m are relatively prime.
 * https://oeis.org/A214714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 950 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the A214714 definition:
   a(n) = smallest m>0 such that gcd( prime(n)-m , n-m ) = 1
   In the code we use the identity gcd( prime(n)-m , n-m )
   = gcd( prime(n)-n , n-m ), so the problem reduces to
   finding the least m>0 such that gcd( |prime(n)-n| , n-m ) = 1.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    /* p = prime(n) – the n-th prime number */
    uint64_t p = (uint64_t)get_prime(n);

    /* g = |p - n|   (non‑negative int of seq_t type) */
    seq_t g;
    if (p >= n)
        g = (seq_t)(p - n);
    else
        g = (seq_t)(n - p);

    /* Find the largest k < n with gcd(g, k) == 1.
       Then m = n - k is the desired value. */
    for (idx_t k = n - 1; k > 0; --k) {
        if (gcd(g, (seq_t)k) == 1)
            return (seq_t)(n - k);
    }

    /* If no k>0 works, use k = 0 (m = n).
       This can happen only when g == 1. */
    if (gcd(g, (seq_t)0) == 1)
        return (seq_t)n;

    /* Should never reach here – return n as a last resort. */
    return (seq_t)n;
}
