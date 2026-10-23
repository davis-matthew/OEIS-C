/*
 * A107740  Number of numbers m such that prime(n) = m + (digit sum of m).
 * https://oeis.org/A107740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of m such that prime(n) = m + digit_sum(m)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a small brute‑force search over m in the range
   [p - 9·digit_count(p), p-1] where p = prime(n).  The nth prime
   is obtained on demand by a simple incremental sieve using trial
   division with the primes found so far. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_uint64.h"

static uint64_t *primes = NULL;        /* array of primes found so far */
static size_t   primes_cap = 0;        /* allocated capacity of 'primes' */
static size_t   primes_count = 0;      /* number of primes stored */
static uint64_t last_checked = 2;      /* last number examined for primality */

/* Ensure that primes[0..n-1] exists; extend the array if necessary. */
static void
ensure_prime_n(size_t n)
{
    if (primes_count >= n)
        return;                              /* already computed */

    /* initialise on first call */
    if (primes_count == 0) {
        primes_cap = 8;
        primes = (uint64_t *)realloc(NULL, primes_cap * sizeof(uint64_t));
        if (!primes) exit(1);                /* fatal error */
        primes[primes_count++] = 2;
        last_checked = 2;
        if (primes_count >= n)
            return;
    }

    for (;;) {
        /* look for next candidate prime */
        do {
            ++last_checked;
            if (last_checked <= 2) last_checked = 3;
        } while (last_checked % 2 == 0);

        /* test divisibility by known primes up to sqrt(candidate) */
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > last_checked)
                break;
            if (last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (primes_count == primes_cap) {
                primes_cap = primes_cap * 2 + 1;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                   primes_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_count++] = last_checked;
            if (primes_count >= n)
                return;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* offset starts at 1 */

    /* obtain the nth prime (1‑based index) */
    ensure_prime_n((size_t)n);
    uint64_t p = primes[n - 1];

    /* number of decimal digits in p */
    uint64_t tmp = p;
    int ndigits = 0;
    do {
        ++ndigits;
        tmp /= 10;
    } while (tmp > 0);

    /* search range for m: [p - 9*ndigits, p-1] */
    uint64_t low = (p > (uint64_t)9 * ndigits) ? p - 9 * ndigits : 0;

    seq_t count = 0;
    for (uint64_t m = low; m < p; ++m) {
        if (m + (uint64_t)digit_sum_uint64(m) == p)
            ++count;
    }
    return count;
}
