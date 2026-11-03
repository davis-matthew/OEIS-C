/*
 * A145280  Fecundity of n-th prime.
 * https://oeis.org/A145280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A070562(prime(n)) where A070562(x) is the number of
 *            iterations of x += product_of_digits(x) needed to reach a number
 *            containing a zero digit. For an n-th prime p, a(n) = number of
 *            such iterations starting from p
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_product.h"

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

/* Check if a number contains a zero digit */
static inline bool has_zero_digit(seq_t v)
{
    while (v > 0) {
        if ((v % (seq_t)10) == 0)
            return true;
        v /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    uint32_t p = get_prime(n);

    seq_t cur = (seq_t)p;
    int steps = 0;

    while (!has_zero_digit(cur)) {
        /* Add the product of the digits to the current value. */
        cur += digit_product(cur);
        steps++;
    }

    return (seq_t)steps;
}
