/*
 * A217254  a(n) = round(primepi(n) * prime(n)/n).
 * https://oeis.org/A217254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = round( primepi(n) * prime(n) / n )
 * Range:     u8 n <= 13, u16 n <= 281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9203 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the built‑in utilities prime_pi() and
 * get_prime() to obtain the prime counting function π(n) and the nth
 * prime p(n).  Rounding of the rational number π(n)∙p(n)/n is
 * performed by adding ⌊n⁄2⌋ before integer division, which implements
 * standard round-to-nearest with ties away from zero for positive
 * operands.  All arithmetic is done in the target type seq_t, which
 * may be 8, 16, 32, 64 or 128 bits, or an arbitrary–precision type,
 * so the code works for every build configuration. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/prime_pi.h"

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
    /* Protect against accidental misuse for n<1 – the sequence is
     * defined only for positive indices.  The caller commits to
     * providing a valid n, so simply return 0 for n==0. */
    if (n == 0)
        return 0;

    /* π(n) – number of primes ≤ n.  The function returns a seq_t. */
    seq_t pi = prime_pi(n);

    /* p(n) – the nth prime.  The utility returns a 32‑bit unsigned
     * integer; cast to seq_t so all following arithmetic uses the
     * same type. */
    seq_t p = (seq_t)get_prime(n);

    /* Convert n to seq_t for the arithmetic below. */
    seq_t nn = (seq_t)n;

    /* Number of primes × nth prime. */
    seq_t numer = pi * p;

    /* Half of n, truncated as integer.  Adding this before division
     * implements round-to-nearest for positive values. */
    seq_t half = nn / 2;

    /* Final rounded quotient. */
    return (numer + half) / nn;
}
