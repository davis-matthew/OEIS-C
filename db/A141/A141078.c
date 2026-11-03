/*
 * A141078  a(n) = abs(prime(n)*prime(n+4) - prime(n+1)*prime(n+3)).
 * https://oeis.org/A141078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = abs(prime(n) * prime(n+4) - prime(n+1) * prime(n+3))
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 72 terms through n = 8017 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* n is 1‑based.  The function is safe for n >= 1.  */
    if (n == 0) return 0;                 /* guard against misuse */

    /* Fetch the five needed primes.  get_prime uses 1‑based indexing. */
    uint32_t p0 = get_prime(n);
    uint32_t p1 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 3);
    uint32_t p4 = get_prime(n + 4);

    /* Compute the two products in seq_t to avoid overflow. */
    seq_t prod1 = (seq_t)p0 * (seq_t)p4;
    seq_t prod2 = (seq_t)p1 * (seq_t)p3;

    /* Return the absolute difference. */
    return (prod1 > prod2) ? (prod1 - prod2) : (prod2 - prod1);
}
