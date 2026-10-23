/*
 * A138558  Expansion of eta(q^2)^7 * eta(q^5)^2 * eta(q^20)^2 / (eta(q)^2 *
 *          eta(q^4)^2 * eta(q^10)^3) in powers of q.
 * https://oeis.org/A138558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. a(1) = 1; For prime powers: a(2^e) =
 *            -(-2)^e (e>0) a(5^e) = 1 if p ≡ 1 or 4 (mod 5): a(p^e) =
 *            (p^(e+1) - 1)/(p - 1) if p ≡ 2 or 3 (mod 5): a(p^e) =
 *            ((-p)^(e+1) - 1)/(-p - 1)
 * Range:     i8 n <= 12, i16 n <= 280, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/pow2.h"

static seq_t prime_power_value(uint64_t p, idx_t e)
{
    if (p == 2)
    {
        seq_t pw = pow2(e);          /* 2^e */
        return (e & 1) ? pw : -pw;   /* -(-2)^e */
    }
    if (p == 5)
        return 1;
    if (p % 5 == 1 || p % 5 == 4)    /* ratio = p */
    {
        seq_t numerator = pow_seq((seq_t)p, e + 1) - 1;
        seq_t denominator = (seq_t)(p - 1);
        return numerator / denominator;
    }
    /* p % 5 == 2 or 3: ratio = -p */
    seq_t numerator = pow_seq(-(seq_t)p, e + 1) - 1;
    seq_t denominator = -(seq_t)p - 1;
    return numerator / denominator;
}

seq_t a(idx_t n)
{
    /* ensure we never evaluate n == 0, but still handle just in case */
    if (n == 0)
        return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t part = prime_power_value(primes[i], exps[i]);
        result *= part;
    }
    return result;
}
