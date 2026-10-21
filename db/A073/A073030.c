/*
 * A073030  Sum_{k=1..n, gcd(n,k) = 1} 10^(k-1).
 * https://oeis.org/A073030
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} 10^(k-1) (OEIS A073030)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd_idx.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    seq_t result = 0;
    seq_t power  = 1;              /* 10^(k-1) for current k */

    for (idx_t k = 1; k <= n; ++k)
    {
        if (gcd_idx(n, k) == 1)
            result += power;
        power *= 10;              /* next power of 10 */
    }
    return result;
}
