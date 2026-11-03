/*
 * A178152  The number of quadratic residues (mod p) greater than p/2, where
 *          p=prime(n).
 * https://oeis.org/A178152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑zero quadratic residues modulo the nth
 *            prime that are > p/2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 0, a(3) = 1, ... as defined by OEIS A178152. */

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
    /* Offsets: a(2) is defined; values for n < 2 are irrelevant. */
    if (n < 2)
        return 0;

    /* Fetch the nth prime. get_prime returns uint32_t, safe to cast to idx_t. */
    idx_t p = get_prime(n);                 /* p = prime(n)   */
    idx_t half = p / 2;                     /* floor(p/2)     */
    idx_t upper = (p - 1) / 2;              /* count distinct residues */

    idx_t count = 0;                        /* count of residues > half */
    for (idx_t k = 1; k <= upper; ++k)
    {
        idx_t r = (k * k) % p;              /* k^2 mod p  (distinct residues for k <= upper) */
        if (r > half)
            ++count;
    }
    return (seq_t)count;
}
