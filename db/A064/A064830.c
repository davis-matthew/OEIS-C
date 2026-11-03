/*
 * A064830  a(n) = gcd(n, prime(n)^2 - 1).
 * https://oeis.org/A064830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, prime(n)^2 - 1)
 * Range:     u8 n <= 263, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 110 terms through n = 953 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Compute the n‑th term of sequence A064830.
   n is 1‑based; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety for off‑by‑one callers */
        return 0;

    /* Obtain the n‑th prime.  get_prime returns a uint32_t. */
    uint32_t p32 = get_prime(n);
    uint64_t p   = (uint64_t)p32;
    uint64_t nn  = (uint64_t)n;

    /* Compute (p^2 – 1) modulo n.  Using modular reduction avoids
       overflow even when p^2 is large. */
    uint64_t p_mod = p % nn;
    uint64_t sq_mod = (p_mod * p_mod) % nn;
    uint64_t val_mod = (sq_mod + nn - 1) % nn;   /* (p^2 – 1) mod n */

    /* The gcd is taken in seq_t.  The result never exceeds n,
       so it fits into seq_t for the tested ranges. */
    return gcd((seq_t)n, (seq_t)val_mod);
}
