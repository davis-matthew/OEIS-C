/*
 * A088198  Distance LQnR(p_n) (A088196) from p_n.
 * https://oeis.org/A088198
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - LQnR(prime(n))
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 99 terms through n = 100 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/powmod.h"

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
        return 0; /* undefined below OEIS offset */

    /* nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* For primes congruent to 3 (mod 4), −1 is a non‑residue,
       so the largest quadratic non‑residue is p−1 and a(n) = 1. */
    if ((uint64_t)p % 4ULL == 3ULL)
        return (seq_t)1;

    /* For primes ≡ 1 (mod 4) we search downward for the first
       quadratic non‑residue using Legendre symbol.  The search
       terminates after a few iterations for the required range of n. */
    seq_t LQnR = p - 1; /* placeholder; will be overwritten */
    uint64_t exp = ((uint64_t)p - 1ULL) >> 1; /* (p‑1)/2 */

    for (seq_t cand = p - 1; cand > 0; --cand)
    {
        seq_t val = powmod(cand, exp, p);
        if (val == p - 1)   /* Legendre symbol = −1 */
        {
            LQnR = cand;
            break;
        }
    }

    return p - LQnR;
}
