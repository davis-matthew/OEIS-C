/*
 * A058974  a(n) = 0 if n = 1 or a prime, otherwise a(n) = s + a(s) iterated
 *          until no change occurs, where s (A008472) is sum of distinct
 *          primes dividing n.
 * https://oeis.org/A058974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1 or n is prime; otherwise a(n) = s + a(s) where
 *            s = sum of distinct prime divisors of n
 * Range:     u8 n <= 397, u16 n <= 9770, u32 n <= 9770, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   factor_uint64, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/isPrime.h"
#include "../utils/factor_uint64.h"

/* Return the sum of distinct prime divisors of x. */
static inline seq_t sum_distinct_primes(idx_t x)
{
    if (x <= 1)
        return 0;

    uint64_t fac[32];
    int     exp[32];
    int     cnt = 0;

    /* Factor x into distinct primes. */
    factor_uint64((uint64_t)x, fac, exp, &cnt);

    seq_t sum = 0;
    for (int i = 0; i < cnt; ++i)
        sum += (seq_t)fac[i];
    return sum;
}

/* Recursive routine that follows the definition. */
static seq_t a_rec(idx_t n)
{
    if (n <= 1 || isPrime(n))
        return 0;

    seq_t s = sum_distinct_primes(n);
    return s + a_rec((idx_t)s);
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
