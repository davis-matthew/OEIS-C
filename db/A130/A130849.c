/*
 * A130849  a(n) is half the sum of the terms in the n-th antidiagonal of the
 *          table A130836.
 * https://oeis.org/A130849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n+1} bigomega(k / gcd(n+1, k))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;          /* n ≥ 1 */
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k)
    {
        idx_t g   = gcd_idx(m, k);
        idx_t rem = k / g;          /* 1 ≤ rem ≤ m */
        sum += bigomega(rem);
    }
    return sum;
}
