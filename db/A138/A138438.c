/*
 * A138438  a(n) = ((n-th prime)^5-(n-th prime)^3)/24.
 * https://oeis.org/A138438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5 - (p_n)^3) / 24
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 43 terms through n = 862 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where p_n is the n-th prime (1‑based)   */

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
    /* Get the n‑th prime as a 64‑bit value */
    uint64_t p = (uint64_t)get_prime(n);

    /* Use 128‑bit arithmetic if the compiler supports it,
       otherwise fall back to seq_t multiplication.       */
#ifdef __SIZEOF_INT128__
    unsigned __int128 mp   = (unsigned __int128)p;
    unsigned __int128 p3   = mp * mp * mp;      /* mp^3 */
    unsigned __int128 p5   = p3 * mp * mp;      /* mp^5 */
    unsigned __int128 val  = (p5 - p3) / 24;     /* (mp^5 - mp^3)/24 */
    return (seq_t)val;
#else
    /* In compilers without __int128, let seq_t handle the arithmetic. */
    seq_t sp  = (seq_t)p;
    seq_t sp3 = sp * sp * sp;                /* sp^3 */
    seq_t sp5 = sp3 * sp * sp;               /* sp^5 */
    seq_t val = (sp5 - sp3) / 24;            /* (sp^5 - sp^3)/24 */
    return val;
#endif
}
