/*
 * A203682  v(n+1)/v(n), where v=A203681.
 * https://oeis.org/A203682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1)) * Product_{d=1..n} (4^d - 2^d + 1), i.e.
 *            v(n+1)/v(n) with v = A203681
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t p = 1;
    for (idx_t d = 1; d <= n; ++d)
    {
        p = p * 2;                      /* 2^d */
        prod = prod * (p * p - p + 1);
    }
    idx_t e = n * (n - 1);
    for (idx_t i = 0; i < e; ++i)
        prod = prod * 2;
    return prod;
}
