/*
 * A167312  Totally multiplicative sequence with a(p) = 2*(p-3) for prime p.
 * https://oeis.org/A167312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is totally multiplicative with a(p) = 2*(p-3) for every
 *            prime p
 * Range:     i8 n <= 66, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n into primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* Any factor 3 yields zero */
        if (primes[i] == 3ULL)
            return (seq_t)0;

        /* base = 2 * (p - 3) (with signed arithmetic) */
        seq_t base = (seq_t)2 * (((seq_t)primes[i]) - (seq_t)3);

        /* (2*(p-3))^e */
        seq_t power = seq_pow(base, (idx_t)exps[i]);

        /* multiply into running product */
        result = seq_mul(result, power);
    }
    return result;
}
