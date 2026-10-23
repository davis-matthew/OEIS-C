/*
 * A105086  Sum of the divisors of n minus the least nontrivial proper divisor
 *          of n.
 * https://oeis.org/A105086
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - least_nontrivial_divisor(n)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   least_divider
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/least_divider.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       // undefined for n < 2 in OEIS definition

    idx_t m = n;
    seq_t sigma = 1;
    idx_t i;

    /* Factorization and multiplicative sigma computation */
    for (i = 2; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            /* Count exponent of prime i */
            idx_t e = 0;
            while (m % i == 0)
            {
                m /= i;
                ++e;
            }

            /* Compute (i^(e+1)-1)/(i-1) by summing powers incrementally */
            seq_t term_pow = 1;
            seq_t term_sum = 1;
            for (idx_t k = 1; k <= e; ++k)
            {
                term_pow *= (seq_t)i;
                term_sum += term_pow;
            }
            sigma *= term_sum;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
    {
        sigma *= ((seq_t)1 + (seq_t)m);
    }

    seq_t least = least_divider(n);
    return sigma - least;
}
