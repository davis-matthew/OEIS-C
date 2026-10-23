/*
 * A017700  Denominator of sum of -18th powers of divisors of n.
 * https://oeis.org/A017700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Denominator of sum of -18th powers of divisors of n. a(n) =
 *            denominator of sigma_18(n) / n^18 in lowest terms (sigma_18(n)
 *            is the sum of d^18 over all divisors d of n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 138,
 *            big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

/* Power of a base raised to an integer exponent (num <= 64-bit). */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* Highest exponent t such that prime^t divides (base^exp - 1).  */
static int val_power(seq_t base, uint64_t exp, seq_t prime)
{
    if (base % prime == 0)
        return 0;          /* if base is a multiple of prime, val is zero */

    int v = 0;
    seq_t mod = prime;
    seq_t rem = mod_pow(base, exp, mod);
    if (rem != 1)
        return 0;

    v = 1;
    while (1) {
        seq_t mod_next = mod * prime;
        seq_t rem_next = mod_pow(base, exp, mod_next);
        if (rem_next == 1) {
            mod = mod_next;
            ++v;
        } else
            break;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* Sum of exponents contributed by sigma_18 to each prime. */
    int exp_sum[64] = {0};

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t r = primes[i];
        uint64_t e = exps[i];
        uint64_t m1 = 18 * (e + 1);   /* exponent for numerator             */
        uint64_t m2 = 18;             /* exponent for denominator           */

        for (size_t j = 0; j < cnt; ++j) {
            uint64_t p = primes[j];
            int v1 = val_power((seq_t)r, m1, (seq_t)p);
            int v2 = val_power((seq_t)r, m2, (seq_t)p);
            int v  = v1 - v2;
            if (v > 0)
                exp_sum[j] += v;
        }
    }

    /* Assemble the denominator: product p^(18*e - min(18*e, exp_sum)) */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e_n = exps[i];
        int      e   = 18 * (int)e_n - exp_sum[i];
        if (e > 0)
            result = result * pow_seq((seq_t)p, (uint64_t)e);
    }

    return result;
}
