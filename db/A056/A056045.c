/*
 * A056045  a(n) = Sum_{d|n} binomial(n,d).
 * https://oeis.org/A056045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n,d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            sum += binomial(n, i);
            idx_t j = n / i;
            if (j != i)
                sum += binomial(n, j);
        }
    }
    return sum;
}
