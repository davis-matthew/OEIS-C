/*
 * A214617  Primes written in the factorial base.
 * https://oeis.org/A214617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007623(A000040(n)) – factorial base representation of
 *            the nth prime
 * Range:     u8 n <= 7, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

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
    if (n < 1) return 0;

    /* Get the nth prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute factorials up to the largest <= p */
    seq_t fact[21];
    fact[0] = 1;
    size_t max_i = 0;
    for (size_t i = 1; ; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
        if (fact[i] > p) {
            max_i = i - 1;
            break;
        }
    }

    seq_t result = 0;
    for (size_t i = max_i; i > 0; --i) {
        seq_t digit = p / fact[i];
        p = p % fact[i];

        /* Append the digit(s) to the decimal result */
        if (digit < 10) {
            result = result * 10 + digit;
        } else {
            /* Split multi‑digit decimal number into its digits */
            seq_t temp = digit;
            unsigned int len = 0;
            while (temp > 0) { len++; temp /= 10; }
            if (len == 0) len = 1; /* for digit == 0 */
            uint8_t digs[32] = {0};
            temp = digit;
            for (unsigned int j = len; j > 0; --j) {
                digs[j-1] = (uint8_t)(temp % 10);
                temp /= 10;
            }
            for (unsigned int j = 0; j < len; ++j) {
                result = result * 10 + digs[j];
            }
        }
    }

    return result;
}
