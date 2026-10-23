/*
 * A171042  In the sequence of prime numbers, replace all the '8' digits with
 *          '1' and vice versa.
 * https://oeis.org/A171042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A171042:
   a(n) is obtained by taking the n‑th prime and swapping every
   digit 1 with 8 and vice versa.
   The routine generates primes on demand with trial division,
   and then transforms the digits of the prime.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* --- generate the first n primes, stored in 64‑bit array --------- */
    static uint64_t *primes       = NULL;
    static size_t   primes_count  = 0;
    static size_t   primes_cap    = 0;
    static uint64_t last_checked  = 1;   /* next odd number to test */

    if (primes == NULL) {                      /* first call */
        primes_cap = 1024;
        primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (primes == NULL) return 0;          /* out of memory (unlikely) */
        primes[0]  = 2;
        primes_count = 1;
    }

    while (primes_count < (size_t)n) {         /* keep generating primes */
        last_checked += 2;                     /* only odd candidates */
        uint64_t candidate = last_checked;
        int is_prime = 1;

        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            if (primes_count == primes_cap) {
                primes_cap <<= 1;              /* grow array */
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (tmp == NULL) return 0;     /* allocation failed */
                primes = tmp;
            }
            primes[primes_count++] = candidate;
        }
    }

    /* --- transform the n‑th prime ----------------------------------- */
    seq_t prime = (seq_t)primes[n - 1];

    /* Count decimal digits of prime (safe for any seq_t size) */
    int digits = 0;
    seq_t tmp = prime;
    while (tmp > 0) { tmp /= 10; ++digits; }
    if (digits == 0) digits = 1;    /* prime is 0 only for n=0, which is unused */

    seq_t result = 0;
    for (int e = digits - 1; e >= 0; --e) {
        seq_t div   = prime / pow10((idx_t)e);   /* extract the e‑th digit */
        uint8_t d   = (uint8_t)(div % 10);
        if (d == 1) d = 8;
        else if (d == 8) d = 1;
        result = result * 10 + d;
    }

    return result;
}
