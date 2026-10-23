/*
 * A033879  Deficiency of n, or 2n - (sum of divisors of n).
 * https://oeis.org/A033879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = 2*n - sigma(n) (sigma(n) = sum of all positive divisors
 *            of n)
 * Range:     i8 n <= 130, i16 n <= 9649, i32 n <= 9649, i64 n <= 9649,
 *            i128 n <= 9649, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Defensive guard for the off‑by‑one offset – OEIS assumes n >= 1. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    /* Iterate over potential divisors without overflowing idx_t. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            sum += (seq_t)i;
            if (i != j)
                sum += (seq_t)j;
        }
    }
    seq_t two_n = (seq_t)2 * (seq_t)n;
    return two_n - sum;
}
