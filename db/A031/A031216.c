/*
 * A031216  Write primes in base 10 but interpret as if in base 11.
 * https://oeis.org/A031216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret decimal digits of prime(n) as base 11
 * Range:     u8 n <= 47, u16 n <= 4696, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Cached list of the first primes encountered */
    static uint32_t *primes   = NULL;
    static size_t  primes_cnt = 0;

    if (n == 0) {              /* offset starts at 1 – for safety */
        return (seq_t)0;
    }

    /* Ensure the cache holds at least n primes */
    if (primes_cnt < (size_t)n) {
        if (primes) {
            free(primes);
            primes   = NULL;
            primes_cnt = 0;
        }
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    /* Retrieve the nth prime (1‑based index) */
    uint32_t prime = primes[n - 1];

    /* Extract its decimal digits, least‑significant first */
    int  digits[20];
    size_t dcount = 0;
    uint32_t tmp = prime;
    while (tmp > 0) {
        digits[dcount++] = (int)(tmp % 10);
        tmp /= 10;
    }
    if (dcount == 0) {         /* prime is 0 – impossible, but guard */
        digits[dcount++] = 0;
    }

    /* Interpret the digits as a base‑11 number (most‑significant first) */
    seq_t value = 0;
    for (size_t i = dcount; i > 0; --i) {
        value = value * 11 + (seq_t)digits[i - 1];
    }

    return value;
}
