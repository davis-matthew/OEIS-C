/*
 * A118271  Expansion of (9 * theta_4(q^3)^4 - theta_4(q)^4) / 8 in powers of
 *          q.
 * https://oeis.org/A118271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 382, i32 n <= 1500, i64 n <= 1500,
 *            i128 n <= 1500, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative formula for OEIS A118271:
   a(0) = 1.
   For prime powers:
     - if p = 2 then a(2^e) = -3   for e > 0
     - if p = 3 then a(3^e) = 4 - 3^(e+1)
     - if p > 3 then a(p^e) = (p^(e+1) - 1) / (p - 1)
   For general n, a(n) = ∏p^e a(p^e)  over all prime powers of n. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

/* Helper that evaluates the prime‐power contribution. */
static seq_t _prime_power_val(idx_t p, idx_t e)
{
    if (p == 2) {
        /* e ≥ 1 */
        return (seq_t)(-3);
    }
    if (p == 3) {
        seq_t term = seq_pow((seq_t)3, e + 1);
        return (seq_t)4 - term;
    }
    /* p > 3 */
    seq_t num = seq_pow((seq_t)p, e + 1) - (seq_t)1;
    seq_t den = (seq_t)p - (seq_t)1;
    return num / den;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= _prime_power_val(p, e);
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        result *= _prime_power_val(temp, 1);
    }

    return result;
}
