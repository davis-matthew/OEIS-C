/*
 * A141454  A Legendre symbol type assignment of the modulo ten primes to the
 *          polynomial: Expand[(x-1)*(x+1)*(x-2)*(x+2)*(x-0)]=4 x - 5 x^3 +
 *          x^5; c(n) = If[Mod[n, 10] == 1, 1, If[Mod[n, 10] == 9, -1,
 *          If[Mod[n, 10] == 3, 2, If[Mod[n, 10] == 7, -2, 0]]]] such that n
 *          is a prime[n].
 * https://oeis.org/A141454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) is the value assigned to the n-th prime, according to its
 *            residue modulo 10: 1 → 1, 9 → –1, 3 → 2, 7 → –2, otherwise → 0
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0)
        return (seq_t)0;    /* unused – sequence starts at n = 1 */

    uint32_t p = get_prime(n);

    switch (p % 10) {
        case 1: return (seq_t) 1;
        case 9: return (seq_t)-1;
        case 3: return (seq_t) 2;
        case 7: return (seq_t)-2;
        default: return (seq_t) 0;
    }
}
