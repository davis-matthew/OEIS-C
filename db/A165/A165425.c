/*
 * A165425  a(1) = 1, a(2) = 7, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=1}^{n-1} a(i) with a(1) = 1, a(2) = 7, n >= 3
 *            (equivalently, a(3)=7 and a(n)=a(n-1)^2 for n>=4)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset */

    if (n == 1)
        return (seq_t)1;

    if (n == 2 || n == 3)
        return (seq_t)7;

    seq_t val = (seq_t)7;
    /* a(4) = 7^2, a(5) = 7^4, a(6) = 7^8, ... */
    for (idx_t i = 4; i <= n; ++i)
        val = val * val;

    return val;
}
