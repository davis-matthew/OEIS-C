/*
 * A084935  Diagonal sums of the array T in A084934.
 * https://oeis.org/A084935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} T(m, n+1-m), where T(m,k) is the k-th
 *            smallest element (1-based) of the multiset {i + m*j : i,j >= 0};
 *            the value x occurs floor(x/m)+1 times in row m
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A084935_T(idx_t m, idx_t k)
{
    idx_t remaining = k;
    for (idx_t val = 0; ; ++val)
    {
        idx_t mult = val / m + 1;
        if (remaining <= mult)
            return val;
        remaining -= mult;
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t m = 1; m <= n; ++m)
        sum += (seq_t)A084935_T(m, n + 1 - m);
    return sum;
}
