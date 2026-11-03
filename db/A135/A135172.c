/*
 * A135172  a(n) = 3^prime(n) + 2^prime(n).
 * https://oeis.org/A135172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^prime(n) + 2^prime(n) (OEIS A135172)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 188 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"

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
    /* OEIS offset is 1 – a(1) is the first term.  If n == 0,
       the sequence is undefined; we return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Get the n‑th prime (1‑based).  get_prime returns a uint32_t,
       which easily fits into idx_t; cast explicitly for clarity. */
    idx_t p = (idx_t)get_prime(n);

    /* Compute 3^p and 2^p using the arbitrary‑precision routine
       pow_uint( base, exponent ).  Then add the two results. */
    seq_t pow3 = ipow_u64_idx(3, p);
    seq_t pow2 = ipow_u64_idx(2, p);
    return pow3 + pow2;
}
