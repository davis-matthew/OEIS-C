/*
 * A072411  LCM of exponents in prime factorization of n, a(1) = 1.
 * https://oeis.org/A072411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = LCM of the exponents in the prime factorization of n
 * Range:     u8 n <= 9592, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* LCM of two positive values (a and b) using the static gcd defined in gcd.h */
static seq_t lcm_seq_t(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);          /* gcd is static in gcd.h */
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1.  For primes or 1 the result is 1. */
    if (n <= 1) return (seq_t)1;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);   /* static in factor_u64.h */

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        result = lcm_seq_t(result, e);
    }
    return result;
}
