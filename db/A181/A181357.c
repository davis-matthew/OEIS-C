/*
 * A181357  Sum of squares of digits of Fibonacci(n).
 * https://oeis.org/A181357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003132(A000045(n)) (sum of squares of digits of the n-th
 *            Fibonacci number)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_counts, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Get the n-th Fibonacci number; a(1) is F(1)=1 */
    seq_t f = fib(n);

    /* Count decimal digits of f */
    unsigned char cnt[10] = {0};
    digit_counts(f, cnt);

    /* Sum the squares of the digits */
    seq_t sum = 0;
    for (unsigned char d = 0; d < 10; ++d)
    {
        seq_t dg = (seq_t)d;
        sum += dg * dg * (seq_t)cnt[d];
    }

    return sum;
}
