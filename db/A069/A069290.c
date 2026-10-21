/*
 * A069290  Sum of the square roots of the square divisors of n.
 * https://oeis.org/A069290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(⌊e/2⌋+1) – 1)/(p – 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Multiplicative formula for the sum of square‑roots of square divisors) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0 */
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 separately */
    idx_t e = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        e++;
    }
    if (e) {
        idx_t t = e >> 1;           /* floor(e/2)   */
        seq_t pow_val = 1;
        for (idx_t k = 0; k <= t; ++k)
            pow_val *= (seq_t)2;    /* 2^(t+1) */
        seq_t numerator = pow_val - 1;
        seq_t denom     = 1;        /* 2-1 = 1    */
        seq_t part      = numerator / denom;
        result *= part;
    }

    /* Odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            idx_t t = e >> 1;       /* floor(e/2)   */
            seq_t pow_val = 1;
            for (idx_t k = 0; k <= t; ++k)
                pow_val *= (seq_t)p; /* p^(t+1)     */
            seq_t numerator = pow_val - 1;
            seq_t denom     = (seq_t)(p - 1);
            seq_t part      = numerator / denom;
            result *= part;
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        e = 1;
        idx_t t = 0;                 /* floor(1/2) = 0 */
        seq_t pow_val = (seq_t)temp; /* temp^(0+1) */
        seq_t numerator = pow_val - 1;
        seq_t denom     = (seq_t)(temp - 1);
        seq_t part      = numerator / denom;
        result *= part;
    }

    return result;
}
