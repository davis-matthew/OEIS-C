/*
 * A086984  Number of arrangements of n labeled balls in n labeled columns
 *          where only 1 column may have more than 1 ball.
 * https://oeis.org/A086984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n * 2^(n-1) - (n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 is not part of the sequence, but handle it for safety. */
    if (n == 0) return (seq_t)1;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute 2^(n-1) efficiently with exponentiation by squaring. */
    seq_t two_pow = 1;
    idx_t e = n - 1;
    seq_t base = 2;
    while (e) {
        if (e & 1U)
            two_pow *= base;
        base *= base;
        e >>= 1;
    }

    /* Compute the factor (n * 2^(n-1) - (n-1)). */
    seq_t tmp = (seq_t)n * two_pow;
    seq_t factor = tmp - (seq_t)(n - 1);

    /* Final result. */
    return fact * factor;
}
