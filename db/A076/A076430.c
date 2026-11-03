/*
 * A076430  Sum of squares of numbers that cannot be written as t*p(n) +
 *          u*p(n+1) for nonnegative integers t,u, where p(n) is the n-th
 *          prime.
 * https://oeis.org/A076430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            p(n)*p(n+1)*(p(n)-1)*(p(n+1)-1)*(p(n)*p(n+1)-p(n)-p(n+1))/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
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
    /* For n < 1 the behavior is undefined; simply return 0 for safety. */
    if (n == 0) return (seq_t)0;
    uint32_t p = get_prime(n);
    uint32_t q = get_prime(n + 1);

    seq_t pseq = (seq_t)p;
    seq_t qseq = (seq_t)q;

    seq_t part1 = pseq * qseq;                         /* p*q */
    seq_t part2 = (pseq - 1) * (qseq - 1);              /* (p-1)*(q-1) */
    seq_t part3 = part1 - pseq - qseq;                 /* p*q - p - q */

    seq_t num  = part1 * part2 * part3;                /* numerator */
    seq_t result = num / (seq_t)12;                    /* divide by 12 */

    return result;
}
