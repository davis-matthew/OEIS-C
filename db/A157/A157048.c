/*
 * A157048  a(n) = the smallest integer > p(n) that is coprime to
 *          (p(n+1)-p(n)), where p(n) is the n-th prime.
 * https://oeis.org/A157048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer > p(n) that is coprime to
 *            (p(n+1)-p(n))
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8773 (2026-11-01)
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

/* Return the n-th term of OEIS sequence A157048.  The index n is
   1‑based; values are computed using the first n prime numbers. */
seq_t a(idx_t n)
{
    /* behaviour for n < 1 is irrelevant for the OEIS offset */
    if (n < 1)
        return 0;

    /* get the n‑th and (n+1)‑st prime numbers (1‑based in get_prime) */
    seq_t p_n   = get_prime(n);
    seq_t p_np1 = get_prime(n + 1);

    /* difference between consecutive primes */
    seq_t diff = p_np1 - p_n;

    /* candidate > p_n */
    seq_t k = p_n + 1;

    /* Find the first k > p_n that is coprime to diff.  gcd() is
       provided by "../utils/gcd.h". */
    while (gcd(k, diff) != (seq_t)1)
        ++k;

    return k;
}
