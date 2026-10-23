/*
 * A119944  First differences of A003418(n) = lcm(1..n).
 * https://oeis.org/A119944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(1..n+1) – lcm(1..n) = 0 unless n+1 is a prime power
 *            p^k, in which case a(n) = lcm(1..n) * (p – 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a static list of primes up to the
    largest argument seen and computes each lcm by multiplying the
    appropriate prime powers.  All multiplications and powers use
    the provided seq_mul and seq_pow helper functions. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Static prime table, rebuilt only when a larger limit is needed. */
static uint64_t *primes      = NULL;
static size_t    primes_cnt  = 0;
static idx_t     primes_limit = 0;

/* Rebuild the prime table for values up to `limit`. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    free(primes);                     /* discard old table if any */
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* Test whether m is a prime power.  If it is, store the prime base in *p_base.*/
static int is_prime_power(idx_t m, uint64_t *p_base)
{
    idx_t temp = m;
    uint64_t p = 0;

    /* trial division using the prime table */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t q = primes[i];
        if (temp % q == 0) {
            p = q;
            while (temp % q == 0) {
                temp /= q;
            }
            break;
        }
    }
    if (p == 0) {          /* m itself is prime */
        p = m;
    } else if (temp != 1) {/* has a second distinct prime factor */
        return 0;
    }

    *p_base = p;
    return 1;
}

/* Compute the lcm of 1..n using prime powers. */
static seq_t lcm_upto(idx_t n)
{
    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        uint64_t q = primes[i];
        idx_t e = 0;
        idx_t pow = q;
        while (pow <= n) {
            ++e;
            if (pow > n / q)      /* avoid overflow of pow * q */
                break;
            pow *= q;
        }
        if (e > 0) {
            seq_t p_to_e = seq_pow((seq_t)q, e);   /* q^e */
            result = seq_mul(result, p_to_e);
        }
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* a(0) = 0 by definition */
        return (seq_t)0;
    }

    /* Ensure enough primes for factoring n+1 and computing lcm(1..n). */
    idx_t m = n + 1;
    idx_t limit = (m > n) ? m : n;
    ensure_primes(limit);

    uint64_t base_prime;
    if (!is_prime_power(m, &base_prime)) {
        return (seq_t)0;
    }

    seq_t lcm_val = lcm_upto(n);
    seq_t factor = (seq_t)(base_prime - 1);
    return seq_mul(lcm_val, factor);
}
