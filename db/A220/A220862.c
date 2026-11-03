/*
 * A220862  a(n) = smallest m>0 such that the n-th rational prime p splits in
 *          the imaginary quadratic extension field K = Q(sqrt(-m)).
 * https://oeis.org/A220862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest positive integer m such that the n‑th prime p
 *            splits in the imaginary quadratic field K = Q(sqrt(-m)). For
 *            n>=2 this equals -A088192(n); the first term is the exceptional
 *            -7
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
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

/* the function to implement */
seq_t a(idx_t n)
{
    /* special first term */
    if (n == 1) return (seq_t)(-7);

    /* get the n-th prime (p >= 3) */
    uint32_t p = get_prime(n);

    /* if p ≡ 1 (mod 4) then m = 1 is the minimal solution */
    if ((p & 3u) == 1u) return (seq_t)(-1);

    /* p ≡ 3 (mod 4): find the largest quadratic residue modulo p,
       then m = p – that residue. */
    uint32_t limit = (p - 1u) >> 1u;   /* iterate a = 1..(p-1)/2 */
    uint64_t p64 = (uint64_t)p;
    uint32_t max_sqr = 0;

    for (uint32_t a = 1; a <= limit; ++a) {
        uint64_t sq = (uint64_t)a * a % p64;
        if (sq > max_sqr) max_sqr = (uint32_t)sq;
    }

    uint32_t m = p - max_sqr;
    return -(seq_t)m;
}
