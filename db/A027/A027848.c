/*
 * A027848  a(n) = Sum_{ d|n } sigma(n/d)*d^4.
 * https://oeis.org/A027848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{ d|n } sigma(n/d) * d^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 243, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* sum of the ordinary divisor function sigma(n) = sum_{d|n} d */
static seq_t sigma(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* d^4 */
            seq_t d4 = (seq_t)d * (seq_t)d * (seq_t)d * (seq_t)d;
            idx_t div = n / d;
            seq_t sigma_val = sigma(div);
            result += sigma_val * d4;
        }
    }
    return result;
}
