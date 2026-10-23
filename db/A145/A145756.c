/*
 * A145756  a(n) = ((2^prime(n+2)-2)/prime(n+2))/3, where n >= 1.
 * https://oeis.org/A145756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^prime(n+2)-2)/prime(n+2))/3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

static size_t estimate_prime_limit(idx_t k)
{
    if (k < 6) return 15;
    double dk = (double)k;
    double limit = dk * (log(dk) + log(log(dk))) + 3.0;
    return (size_t)limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static uint64_t *primes = NULL;
    static size_t primes_count = 0;

    idx_t k = n + 2;                 /* we need prime(k) */
    if (primes_count < (size_t)k) {
        /* increase prime list until it contains at least k primes */
        size_t limit = estimate_prime_limit(k);
        uint64_t *new_primes = NULL;
        size_t new_count = 0;
        build_primes_upto((idx_t)limit, &new_primes, &new_count);
        /* if not enough, double the limit until sufficient */
        while (new_count < (size_t)k) {
            free(new_primes);
            limit = estimate_prime_limit(new_count + 100);
            build_primes_upto((idx_t)limit, &new_primes, &new_count);
        }
        free(primes);
        primes = new_primes;
        primes_count = new_count;
    }

    uint64_t p = primes[k - 1];

    /* compute ((2^p-2)/p)/3   using a safe method depending on seq_t size */
    if (sizeof(seq_t) <= 8) {
        /* use 128‑bit intermediate for the 64‑bit build */
        __int128 power = (__int128)1 << p;          /* p <= 127 for any 64‑bit result */
        __int128 val   = power - (__int128)2;
        val /= (__int128)p;
        val /= (__int128)3;
        return (seq_t)val;
    } else {
        /* arbitrary‑precision build: pow2 returns an exact seq_t */
        seq_t power = pow2((idx_t)p);
        seq_t val   = power - (seq_t)2;
        val /= (seq_t)p;
        val /= (seq_t)3;
        return val;
    }
}
