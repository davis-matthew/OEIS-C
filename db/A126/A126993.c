/*
 * A126993  a(n) = binomial(prime(n+3), prime(n)).
 * https://oeis.org/A126993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( prime(n+3), prime(n) ) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 22, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 932 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* primes are 1‑indexed in the OEIS definition */
    idx_t p = (idx_t)get_prime(n);     /* prime(n)   */
    idx_t q = (idx_t)get_prime(n + 3); /* prime(n+3) */

    /* binomial(q, p) = binomial(q, q-p).  Let k = q-p. */
    idx_t k = q - p;
    if (k == 0) /* p == q, happens only for n=1 (primes 2 and 5) */
        return 1;

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* Numerator term (q - k + i) and denominator i */
        seq_t num = (seq_t)(q - k + i);
        seq_t den = (seq_t)i;

        /* Reduce gcd(num,den) first */
        seq_t g = gcd(num, den);
        num /= g;     den /= g;

        /* Reduce gcd(result,den) */
        g = gcd(result, den);
        result /= g;  den /= g;

        /* After reductions, den should be 1.
           Multiply the remaining numerator. */
        /* If den is still >1, multiply then divide – it must be exact */
        result = result * num;
        if (den != 1)
            result = result / den;
    }

    return result;
}
