/*
 * A086917  a(n) = floor(prime(n) - n*(log(n) - log(log(n)) - 1)) for n>=2.
 * https://oeis.org/A086917
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n) - n*(log(n) - log(log(n)) - 1) ) for n >=
 *            2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Store primes in a static 64‑bit array – prime(10000) < 2^64 */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static idx_t last_checked = 0;

    if (!primes) {
        primes_cap = 128;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return 0;
    }

    /* Extend prime list until it has n elements */
    while (primes_len < n) {
        idx_t candidate;
        if (primes_len == 0) {
            candidate = 2;
        } else {
            candidate = last_checked + 1;
        }
        last_checked = candidate;

        /* Skip even numbers > 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0)
            continue;

        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            idx_t p = (idx_t)primes[i];
            if (p > candidate / p) break;
            if (candidate % p == 0) {
                isprime = false;
                break;
            }
        }

        if (isprime) {
            if (primes_len >= primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) return 0;
                primes = tmp;
            }
            primes[primes_len++] = candidate;
        }
    }

    uint64_t prime_n = primes[n - 1];

    long double nd = (long double) n;
    long double logn = logl(nd);
    long double loglogn = logl(logn);
    long double expr = nd * (logn - loglogn - 1.0L);
    long double diff = (long double)prime_n - expr;
    long double fl = floorl(diff + 1e-12L); /* small epsilon to avoid rounding issues */

    uint64_t val = (uint64_t) fl;
    return (seq_t) val;
}
