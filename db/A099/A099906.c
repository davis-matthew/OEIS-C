/*
 * A099906  a(n) = binomial(2n-1,n-1) mod n^2.
 * https://oeis.org/A099906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n-1, n-1) mod n^2
 * Range:     u8 n <= 15, u16 n <= 271, u32 n <= 1885, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    /* Prime table, built once. */
    static idx_t *primes = NULL;
    static size_t primeCnt = 0;
    static bool initialized = false;
    if (!initialized) {
        build_primes_upto((idx_t)20000, &primes, &primeCnt);
        initialized = true;
    }

    seq_t mod   = (seq_t)n * (seq_t)n;
    seq_t result = 1;
    idx_t limit = 2 * n - 1;

    for (size_t i = 0; i < primeCnt && primes[i] <= limit; ++i) {
        idx_t p = primes[i];
        unsigned long long exp_num = 0, exp_den1 = 0, exp_den2 = 0, t;

        /* Exponent of p in (2n-1)! */
        t = limit;
        while (t) { t /= p; exp_num += t; }

        /* Exponent of p in (n-1)! */
        t = n - 1;
        while (t) { t /= p; exp_den1 += t; }

        /* Exponent of p in n! */
        t = n;
        while (t) { t /= p; exp_den2 += t; }

        unsigned long long exp = exp_num - exp_den1 - exp_den2;
        if (exp == 0) continue;

        seq_t powv = mod_pow((seq_t)p, exp, mod);
        result = (result * powv) % mod;
    }

    return result;
}
