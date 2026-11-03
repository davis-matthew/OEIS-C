/*
 * A213982  Least k >= 1 such that prime(n) +- k = 2^m * q, m >= 0, where q >=
 *          2 is prime.
 * https://oeis.org/A213982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that prime(n) ± k = 2^m * q, where m >=
 *            0 and q >= 2 is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 7911 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

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

static bool _is_form(idx_t y) {
    if (y < 2) return false;
    idx_t odd = y;
    /* Remove all factors of 2. */
    while ((odd & 1) == 0) odd >>= 1;
    /* If the remaining part is 1, y is a power of two, which qualifies
       because we can take q = 2 (prime) and appropriate m. */
    if (odd == 1) return true;
    /* Otherwise, accept only if the odd part is prime. */
    return is_prime(odd);
}

seq_t a(idx_t n) {
    /* Get the n-th prime (1‑based). */
    idx_t p = get_prime(n);

    for (idx_t k = 1; ; ++k) {
        /* Check prime - k if it stays >= 2. */
        if (k <= p - 2) {
            idx_t y = p - k;
            if (_is_form(y)) return (seq_t)k;
        }
        /* Check prime + k. */
        idx_t y = p + k;
        if (_is_form(y)) return (seq_t)k;
    }
}
