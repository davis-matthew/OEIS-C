/*
 * A231505  a(n) = Sum_{i=0..n} digsum_3(i)^4, where digsum_3(i) = A053735(i).
 * https://oeis.org/A231505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digit_sum_base3(i))^4
 * Range:     u8 n <= 7, u16 n <= 124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digit_sum_base3(i) is the sum of the digits of i when written in base 3 (OEIS A053735) */

#include <stdint.h>

/* Helper function that returns the digit sum of x in base 3. */
static inline seq_t digsum3(idx_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        sum += x % 3;
        x /= 3;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t i = 0;
    for (;;)
    {
        seq_t d = digsum3(i);
        seq_t d4 = d * d * d * d;   /* d^4 */
        total += d4;

        if (i == n)
            break;
        ++i;
    }
    return total;
}
