/*
 * A157754  a(1) = 0, a(n) = lcm(A051904(n), A051903(n)) for n >= 2.
 * https://oeis.org/A157754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( minimum exponent in prime factorization of n,
 *            maximum exponent in prime factorization of n ); a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0)   /* should only happen for n == 1 */
        return (seq_t)0;

    seq_t minExp = exps[0];
    seq_t maxExp = exps[0];

    for (size_t i = 1; i < cnt; ++i) {
        seq_t e = exps[i];
        if (e < minExp) minExp = e;
        if (e > maxExp) maxExp = e;
    }

    seq_t g   = gcd(minExp, maxExp);
    seq_t lcm = (minExp / g) * maxExp;

    return lcm;
}
