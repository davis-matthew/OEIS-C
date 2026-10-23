/*
 * A036502  Numerator of n^(n-2)/n!.
 * https://oeis.org/A036502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction n^(n-2)/n!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n, cancels common powers with n! and
   multiplies the remaining prime powers to give the numerator. */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t temp_n = n;                     /* copy for factorisation */

    /* Factorise n */
    idx_t p = 2;
    while (p * p <= temp_n) {
        if (temp_n % p == 0) {
            /* exponent of p in n */
            idx_t a_p = 0;
            while (temp_n % p == 0) {
                temp_n /= p;
                ++a_p;
            }

            /* exponent of p in n^(n-2) */
            idx_t exp_num = a_p * (n - 2);

            /* exponent of p in n! */
            idx_t exp_den = 0;
            idx_t power = p;
            while (power <= n) {
                exp_den += n / power;
                /* prevent overflow of power *= p   */
                if (power > n / p) break;
                power *= p;
            }

            idx_t diff = (exp_num > exp_den) ? (exp_num - exp_den) : 0;
            if (diff)
                result *= seq_pow((seq_t)p, diff);
        }
        ++p;
    }

    /* Remaining prime factor larger than sqrt(n) */
    if (temp_n > 1) {
        idx_t p = temp_n;
        idx_t a_p = 1;

        idx_t exp_num = a_p * (n - 2);

        idx_t exp_den = 0;
        idx_t power = p;
        while (power <= n) {
            exp_den += n / power;
            if (power > n / p) break;
            power *= p;
        }

        idx_t diff = (exp_num > exp_den) ? (exp_num - exp_den) : 0;
        if (diff)
            result *= seq_pow((seq_t)p, diff);
    }

    return result;
}
