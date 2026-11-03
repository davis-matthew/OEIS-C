/*
 * A042939  Absolute values between digits of primes.
 * https://oeis.org/A042939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = |(first digit of the nth prime) - (sum of the remaining
 *            digits)|
 * Range:     u8 n <= 54, u16 n <= 6882, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9678 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Obtain the nth prime (1‑based) */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;

    /* Find the first digit of the prime */
    seq_t tmp = prime;
    seq_t first;
    while (tmp >= (seq_t)10)
    {
        tmp /= (seq_t)10;
    }
    first = tmp;

    /* Sum all digits of the prime */
    seq_t sum_digits = 0;
    seq_t tmp2 = prime;
    while (tmp2 > 0)
    {
        sum_digits += tmp2 % (seq_t)10;
        tmp2 /= (seq_t)10;
    }

    /* Sum of remaining digits */
    seq_t sum_rest = sum_digits - first;

    /* Absolute difference between first digit and sum of the rest */
    seq_t diff;
    if (first >= sum_rest)
        diff = first - sum_rest;
    else
        diff = sum_rest - first;

    return diff;
}
