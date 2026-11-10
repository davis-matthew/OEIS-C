/*
 * A073934  Sum of terms in n-th row of triangle in A073932.
 * https://oeis.org/A073934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n + a(n - n/spf(n)), spf = smallest
 *            prime factor (A060681)
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9823 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A060681_step(idx_t m)
{
    if ((m & 1) == 0)
        return m - m / 2;
    for (idx_t i = 3; i <= m / i; i += 2)
        if (m % i == 0)
            return m - m / i;
    return m - 1;   /* m prime */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;
    while (m > 1) {
        sum += (seq_t)m;
        m = A060681_step(m);
    }
    sum += 1;       /* a(1) = 1 */
    return sum;
}
