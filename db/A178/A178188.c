/*
 * A178188  a(n) = Sum_{k=1..n} 5^((k^2+3k)/2).
 * https://oeis.org/A178188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 5^((k^2+3k)/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   power5, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; safety */
    seq_t current = power5(2);      /* 5^((1^2+3*1)/2) = 5^2 = 25 */
    seq_t sum = current;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* exponent increments by k+1 each step: e(k) - e(k-1) = k+1 */
        current = seq_mul(current, power5(k + 1));
        sum += current;
    }

    return sum;
}
