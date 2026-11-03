/*
 * A065112  Smallest prime whose decimal expansion ends (nontrivially) with
 *          the n-th prime; or 0 if no such prime exists.
 * https://oeis.org/A065112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime > prime(n) whose decimal expansion ends
 *            with prime(n); returns 0 if no such prime exists (this occurs
 *            when prime(n) ends with 2 or 5)
 * Range:     u8 n <= 6, u16 n <= 168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9889 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

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
    if (n < 1)
        return 0;                     // undefined, but safe fallback

    /* obtain the n‑th prime (fits in 32 bits for n ≤ 10000) */
    uint32_t p = get_prime(n);

    /* if the prime ends with 2 or 5, no larger prime can end with it */
    uint32_t last = p % 10;
    if (last == 2 || last == 5)
        return 0;

    /* determine number of decimal digits of p */
    uint32_t tmp = p;
    uint32_t d = 0;
    do { d++; tmp /= 10; } while (tmp);

    /* compute 10^d as seq_t */
    seq_t pow10 = 1;
    for (uint32_t i = 0; i < d; ++i)
        pow10 *= 10;

    /* search for the first prime > p ending with p */
    seq_t candidate = p + pow10;      /* k = 1 */
    for (;;)
    {
        if (is_prime((idx_t)candidate))
            return candidate;
        candidate += pow10;           /* next candidate with same suffix */
    }
}
