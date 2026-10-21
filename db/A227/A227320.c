/*
 * A227320  Binary XOR of proper divisors of n.
 * https://oeis.org/A227320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = XOR of all proper divisors of n
 * Range:     u8 n <= 511, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is 0 by definition (no proper divisors) */
    if (n <= 1)
        return 0;

    /* 1 is a proper divisor of every n > 1 */
    seq_t res = (seq_t)1;

    /* Enumerate the remaining proper divisors and XOR them into the result */
    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t j = n / i;          /* complementary divisor */
            res ^= (seq_t)i;           /* XOR the divisor i */
            if (i != j)
                res ^= (seq_t)j;       /* XOR the complementary divisor j */
        }
    }

    return res;
}
