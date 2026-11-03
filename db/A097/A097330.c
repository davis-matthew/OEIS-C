/*
 * A097330  In the sequence of prime numbers replace each term p with
 *          floor(p/2) and ceiling(p/2).
 * https://oeis.org/A097330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n < 3, otherwise (prime(((n)+(n mod 2))/2) + 1)/2 -
 *            (n mod 2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
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
    /* The first three terms are all 1. */
    if (n < 3u)
        return 1;

    /* k = ceil(n/2) = (n+1)/2 gives the index of the prime to use. */
    idx_t k = (n + 1u) >> 1;               /* equivalent to (n + 1) / 2 */

    /* Fetch the k-th prime (uint32_t is sufficient). */
    uint32_t pk = get_prime(k);

    /* Compute floor(pk/2) and ceil(pk/2). */
    seq_t floor_half = (seq_t)(pk >> 1);                 /* pk / 2 */
    seq_t ceil_half  = (seq_t)((pk + 1u) >> 1);          /* (pk + 1) / 2 */

    /* Return according to parity of n: odd -> floor, even -> ceil. */
    return (n & 1u) ? floor_half : ceil_half;
}
