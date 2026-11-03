/*
 * A176266  Binomial(prime(n),s)/prime(n) where s is the sum of the decimal
 *          digits of n.
 * https://oeis.org/A176266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial(prime(n),digit_sum(n))/prime(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 17, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"

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

/* Sum of decimal digits of n, where n fits in idx_t (holds 64 bits) */
static idx_t digit_sum_idx(idx_t n)
{
    idx_t sum = (idx_t)0;
    while (n != (idx_t)0) {
        sum += n % (idx_t)10;
        n   /= (idx_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;       /* undefined for n < 1 */

    /* nth prime */
    idx_t p = (idx_t)get_prime(n);

    /* sum of decimal digits of n */
    idx_t s = digit_sum_idx(n);

    /* binomial(p, s) */
    seq_t b = binomial(p, s);

    /* division by prime p */
    return b / (seq_t)p;
}
