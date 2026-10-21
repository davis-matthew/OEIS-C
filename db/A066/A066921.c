/*
 * A066921  a(n) = lcm(Omega(n), omega(n)).
 * https://oeis.org/A066921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(Omega(n), omega(n)) where Omega(n) = number of prime
 *            factors of n (with multiplicity) omega(n) = number of distinct
 *            prime factors of n The implementation uses factor_u64 to get the
 *            factorisation, then computes the small lcm
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"          /* factor_u64 */
#include "../utils/gcd.h"                /* gcd(seq_t,seq_t) */

seq_t a(idx_t n)
{
    /* n = 1 has no prime factors, a(1) = 0 by definition */
    if (n < 2)
        return 0;

    /* factor_u64 writes prime factors and their exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t omega_total = 0;      /* Omega(n) */
    seq_t omega_distinct = 0;   /* omega(n) */
    for (size_t i = 0; i < cnt; ++i) {
        omega_total += exps[i];
        ++omega_distinct;
    }

    /* Compute lcm(omega_total, omega_distinct) safely */
    seq_t g = gcd(omega_total, omega_distinct);
    seq_t lcm = (omega_total / g) * omega_distinct;

    return lcm;
}
