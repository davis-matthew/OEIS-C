/*
 * A080525  First column of triangle in A080524.
 * https://oeis.org/A080525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) - (n-1)/2 if n is odd; a(n) = n^(n-1) - (n-1) if
 *            n is even
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t power = 1;
    for (idx_t k = 1; k < n; ++k)
        power *= base;
    if ((n & 1) == 1)
        return power - (seq_t)((n - 1) / 2);
    return power - (seq_t)(n - 1);
}
