/*
 * A048795  Concatenate p p times, where p runs through the primes.
 * https://oeis.org/A048795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the nth prime p, repeated p times. *
 *            That is * a(n) = p × (10^(len(p) · p) – 1) / (10^len(p) – 1) *
 *            where len(p) is the number of decimal digits of p
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   ipow, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"
#include "../utils/num_digits_idx.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    /* n ≥ 1 as per OEIS offset */
    uint32_t p = get_prime(n);          /* nth prime, fits in 32‑bit */
    idx_t lenp = num_digits_idx(p);             /* number of decimal digits of p */

    /* Compute the denominator 10^lenp – 1  */
    seq_t ten_len = pow10(lenp);        /* 10^lenp  */
    seq_t denom   = ten_len - 1;        /* 10^lenp – 1 */

    /* Compute the numerator 10^(lenp·p) – 1  */
    seq_t exp     = (seq_t)lenp * (seq_t)p;      /* exponent = lenp * p */
    seq_t numer   = ipow((seq_t)10, exp) - 1; /* 10^exp – 1 */

    /* Assemble the final value */
    seq_t q = numer / denom;            /* division is exact */
    return q * (seq_t)p;                /* multiply by the prime p */
}
