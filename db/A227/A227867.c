/*
 * A227867  Number of Lipschitz quaternions X such that X^2 == 1 (mod n).
 * https://oeis.org/A227867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of Lipschitz quaternions X modulo n with X^2 == 1
 *            (mod n) a(n) is multiplicative. For odd prime powers p^e (e >=
 *            1): a(p^e) = p^(2e) + p^(2e-1) + 2 For powers of two: a(1) = 1
 *            a(2) = 8 a(2^e) = 32 for e >= 2 Thus for an arbitrary n = 2^k *
 *            ∏ p_i^e_i (p_i odd primes) we compute a(n)...
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

static seq_t power_of_prime(idx_t p, idx_t e)
{
    /* Compute p^e and p^(2e) using seq_pow */
    seq_t pe = seq_pow((seq_t)p, e);     /* p^e */
    seq_t pe2 = pe * pe;                 /* p^(2e) */
    seq_t term = pe2 + (pe2 / (seq_t)p) + (seq_t)2;  /* p^(2e)+p^(2e-1)+2 */
    return term;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    idx_t tmp = n;

    /* Handle 2-adic part */
    idx_t exp2 = 0;
    while ((tmp & 1) == 0)
    {
        tmp >>= 1;
        exp2++;
    }
    if (exp2 == 1)          result *= (seq_t)8;   /* a(2) = 8   */
    else if (exp2 >= 2)     result *= (seq_t)32;  /* a(2^e) = 32 for e >= 2 */

    /* Factor remaining odd part */
    for (idx_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                e++;
            }
            result *= power_of_prime(p, e);
        }
    }
    /* Remaining prime > sqrt(tmp) */
    if (tmp > 1)
    {
        /* tmp is odd prime, exponent e = 1 */
        result *= power_of_prime(tmp, 1);
    }

    return result;
}
