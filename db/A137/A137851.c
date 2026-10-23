/*
 * A137851  a(n) = A054525(n) * A061397(n).
 * https://oeis.org/A137851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{p|n} p * mu(n/p)
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset: 1 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"    /* provides uint64_t _smallest_prime_factor(uint64_t) */

seq_t a(idx_t n)
{
    if (n <= 1)  /* a(1) = 0 */
        return (seq_t)0;

    /* Factor n into primes and their exponents */
    uint64_t primes[64];
    int      exps[64];
    int      k = 0;           /* number of distinct prime factors */

    uint64_t temp = n;
    while (temp > 1)
    {
        uint64_t p = _smallest_prime_factor(temp);
        int e = 0;
        do
        {
            temp /= p;
            ++e;
        } while (temp % p == 0);
        primes[k] = p;
        exps[k] = e;
        ++k;
    }

    /* Analyse exponents */
    int count_two = 0;   /* number of primes with exponent exactly 2 */
    for (int i = 0; i < k; ++i)
    {
        if (exps[i] == 2)
            ++count_two;
        else if (exps[i] > 2)   /* an exponent >= 3 forces all terms to be zero */
            return (seq_t)0;
    }

    /* If more than one exponent equals 2, all terms vanish */
    if (count_two > 1)
        return (seq_t)0;

    seq_t result = 0;

    if (count_two == 1)   /* exactly one exponent is 2, others are 1 */
    {
        /* Find the prime whose exponent is 2 */
        seq_t p_two = 0;
        for (int i = 0; i < k; ++i)
            if (exps[i] == 2)
                p_two = (seq_t)primes[i];

        /* mu(n/p) = (-1)^k  for that prime */
        seq_t sign = (k % 2 == 0) ? 1 : -1;
        result = p_two * sign;
    }
    else                   /* all exponents are 1 (n is square‑free) */
    {
        /* mu(n/p) = (-1)^(k-1) for any prime divisor p */
        seq_t sign = (k % 2 == 1) ? 1 : -1;   /* (-1)^(k-1) */
        for (int i = 0; i < k; ++i)
            result += (seq_t)primes[i] * sign;
    }

    return result;
}
