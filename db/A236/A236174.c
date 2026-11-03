/*
 * A236174  Maximal prime among the base-k representations of the n-th prime,
 *          read in decimal, for k=2,3,...,10.
 * https://oeis.org/A236174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = maximal prime among the base-k representations of the
 *            n-th prime (k = 2,...,10)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 180, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 66 terms through n = 1923 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (1-indexed). */
    uint32_t p = get_prime(n);

    /* Temporary buffer for digits of p in base k. */
    uint8_t digits[64];
    seq_t best = 0;

    /* For each base k from 2 to 10. */
    for (int k = 2; k <= 10; ++k) {
        /* Convert p to base k. */
        uint32_t tmp = p;
        int len = 0;
        do {
            digits[len++] = tmp % k;
            tmp /= k;
        } while (tmp != 0);

        /* Interpret the digits as a decimal number. */
        seq_t val = 0;
        for (int i = len - 1; i >= 0; --i) {
            val = val * 10 + digits[i];
        }

        /* If this decimal value is prime and larger than current best,
           record it. */
        if (isPrimeGeneric(val) && val > best) {
            best = val;
        }
    }

    return best;
}
