/*
 * A036452  a(n) = d(d(d(d(n)))), the 4th iterate of number-of-divisors
 *          function with initial value of n.
 * https://oeis.org/A036452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(d(d(d(n)))) (fourth iterate of the divisor-counting
 *            function)
 * Range:     u8 n <= 859, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Computes the number of positive divisors of m (including 1 and m) */
static seq_t divisor_count(idx_t m)
{
    seq_t count = 1;
    idx_t p, e, tmp = m;

    for (p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            count *= (seq_t)(e + 1);
        }
    }
    if (tmp > 1)          /* remaining prime factor */
        count *= (seq_t)2;
    return count;
}

/* Main sequence function requested in the specification */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    for (int i = 0; i < 4; ++i)
        x = divisor_count((idx_t)x);
    return x;
}
