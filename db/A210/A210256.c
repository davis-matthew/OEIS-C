/*
 * A210256  Differences of the sum of distinct values of {floor(n/k),
 *          k=1,...,n}.
 * https://oeis.org/A210256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n+1) - S(n) where S(m) = sum of all distinct values of
 *            ⌊m/k⌋, k = 1 … m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 8675 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t sum_distinct_floor(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; )
    {
        idx_t q   = n / i;               /* quotient ⌊n/i⌋, fits in idx_t */
        sum += (seq_t)q;                 /* add the distinct value once   */
        idx_t next = n / q + 1;          /* first i that yields a smaller quotient */
        i = next;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t np1 = n + 1;                   /* n+1 is safe for the intended range */
    return sum_distinct_floor(np1) - sum_distinct_floor(n);
}
