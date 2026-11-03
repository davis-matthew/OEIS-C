/*
 * A137779  Number of bases (numbering systems, including unary) in which the
 *          n-th prime is a palindrome having at least two digits.
 * https://oeis.org/A137779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = A126071(prime(n))
 * Range:     u8 n <= 54, u16 n <= 6577, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9825 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of bases (including unary) in which the n‑th prime is
   a palindrome having at least two digits. */

/* Required utilities are supplied by the build system. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_palindrome_base.h"

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
    /* the offset of the sequence is 1, so n==0 is an unused case */
    if (n == 0) return 0;

    /* Retrieve the n‑th prime.  get_prime returns a 32‑bit value. */
    uint32_t p32 = get_prime(n);
    idx_t    p   = (idx_t)p32;          /* for loop calculations */

    /* base 1 (unary) always yields a palindrome with at least two digits */
    seq_t count = 1;

    /* base p-1 gives the two‑digit palindrome “11”; count it only when
       p > 2 to avoid double counting the unconditional base 1 for p=2. */
    if (p32 > 2) ++count;

    /* Any palindrome of length ≥ 3 must have base ≤ √p.  Therefore
       we test only bases 2 … floor(√p).  The loop condition b*b ≤ p
       guarantees this. */
    for (idx_t b = 2; b * b <= p; ++b)
    {
        if (is_palindrome_base((seq_t)p32, b))
            ++count;
    }

    return count;
}
