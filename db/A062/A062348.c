/*
 * A062348  a(n) = n! / (number of distinct prime divisors of n).
 * https://oeis.org/A062348
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! / omega(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A062348, where omega(n) is the number of distinct prime factors of n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        /* The sequence is defined for n >= 2. Return 0 for safety. */
        return (seq_t)0;
    }

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute omega(n): count distinct prime divisors */
    idx_t temp = n;
    idx_t omega = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++omega;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    if (temp > 1) {
        ++omega;
    }

    /* division by omega(n) */
    seq_t denom = (seq_t)omega;
    return fact / denom;
}
