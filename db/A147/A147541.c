/*
 * A147541  Result of using the primes as coefficients in an infinite
 *          polynomial series in x and then expressing this series as
 *          (1+x)(1+a(1)*x)(1+a(2)*x^2) ... .
 * https://oeis.org/A147541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Recursively factor the series 1+prime(1)x+prime(2)x^2+... as
 *            (1+x)(1+a(1)x)(1+a(2)x^2).... a(n) is the coefficient of x^n in
 *            the quotient after removing all previous factors
 * Range:     i8 n <= 21, i16 n <= 37, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

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
    if (n == 0) return 0;                          /* offset 1: not used */
    size_t N = (size_t)n;                           /* cast to size_t for loops */

    /* Allocate array for coefficients up to degree N */
    seq_t *coeff = mem_new(seq_t, (N + 1));
    if (!coeff) return 0;                           /* allocation failed */

    coeff[0] = (seq_t)1;                            /* constant term */

    /* Initialise with the prime series */
    for (size_t i = 1; i <= N; ++i)
        coeff[i] = (seq_t)get_prime((idx_t)i);

    /* Divide by (1 + x) */
    for (size_t i = 1; i <= N; ++i)
        coeff[i] = coeff[i] - coeff[i - 1];

    /* Successively remove factors (1 + a(k) x^k) for k = 1 .. N-1 */
    for (size_t k = 1; k < N; ++k) {
        seq_t ak = coeff[k];                        /* current a(k) */
        for (size_t i = k; i <= N; ++i)
            coeff[i] = coeff[i] - ak * coeff[i - k];
    }

    seq_t result = coeff[N];
    mem_free(coeff);
    return result;
}
