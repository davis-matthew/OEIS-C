/*
 * A172216  Smallest k such that sum of digits of prime(n)^k is prime.
 * https://oeis.org/A172216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the sum of digits of prime(n)^k is
 *            prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses decimal big‑integer multiplication so it works for any   */
/* seq_t width, including 64‑bit where prime^k may overflow native integers. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"

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

/* Find the smallest exponent k so that the sum of digits of p^k is prime.  */
seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑indexed). */
    uint32_t p = get_prime(n);
    /* Decimal representation of the current power, little‑endian. */
    unsigned char digits[256];
    size_t len = 1;
    digits[0] = 1;                    /* start with 1 (p^0) */

    for (idx_t k = 1; ; ++k) {
        /* Multiply the current value by p. */
        unsigned long long carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned long long prod = (unsigned long long)digits[i] * p + carry;
            digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry /= 10;
        }

        /* Compute the sum of digits. */
        unsigned sum = 0;
        for (size_t i = 0; i < len; ++i) sum += digits[i];

        /* Check if the sum is prime. */
        if (is_prime_seq((seq_t)sum))
            return (seq_t)k;
    }
}
