/*
 * A238535  Sum of divisors d of n where d > sqrt(n).
 * https://oeis.org/A238535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of n where d > sqrt(n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* x = A000203(n) - A066839(n)  (sigma(n) minus sum of divisors up to floor(sqrt(n))) */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid)   /* mid*mid > n */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = int_sqrt(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= r; ++d) {
        if (n % d == 0) {
            if (d * d == n)
                continue;                      /* perfect square → divisor not counted */
            sum += ((seq_t)n) / ((seq_t)d);    /* complementary divisor > sqrt(n) */
        }
    }
    return sum;
}
