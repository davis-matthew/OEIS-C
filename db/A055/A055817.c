/*
 * A055817  a(n) = T(2n+5,n), array T as in A055807.
 * https://oeis.org/A055817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(2n+5,n) = Sum_{s=1..n+5} binomial(n+5,s) *
 *            binomial(s+n-2,n-1); a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t m = n + 5;
    seq_t sum = 0;

    for (idx_t s = 1; s <= m; ++s) {
        seq_t c1 = binom(m, s);
        seq_t c2 = binom(s + n - 2, n - 1);
        sum = sum + c1 * c2;
    }

    return sum;
}
