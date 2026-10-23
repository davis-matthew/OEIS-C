/*
 * A116607  Sum of the divisors of n which are not divisible by 9.
 * https://oeis.org/A116607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of the divisors of n that are not divisible by 9
 *            (OEIS A116607). The function factors n by trial division and
 *            uses the multiplicative formula: a(3^e) = 4 (for any e > 0)
 *            a(p^e) = (p^(e+1) - 1) / (p - 1) for p ≠ 3 The values for
 *            composite n are the product of the values for the prime‐po...
 * Range:     u8 n <= 16, u16 n <= 877, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Small power routine for seq_t (operand types are seq_t and idx_t). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp > 0) {
        r *= base;
        exp--;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0 – return 0 */
        return 0;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;          /* use native type for trial division */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }

            seq_t term;
            if (p == 3) {
                /* a(3^e) = 4 for any e > 0 */
                term = 4;
            } else {
                /* (p^(e+1) - 1) / (p - 1) */
                seq_t num = pow_seq((seq_t)p, (idx_t)(e + 1));
                num -= 1;
                seq_t den = (seq_t)(p - 1);
                term = num / den;
            }
            result *= term;
        }
    }

    if (temp > 1) {          /* last prime factor > sqrt(original n) */
        seq_t term;
        if (temp == 3) {
            term = 4;
        } else {
            /* exponent e = 1 */
            seq_t num = pow_seq((seq_t)temp, 2);   /* temp^2 */
            num -= 1;
            seq_t den = (seq_t)(temp - 1);
            term = num / den;                    /* equals temp + 1 */
        }
        result *= term;
    }

    return result;
}
