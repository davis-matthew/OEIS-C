/*
 * A186446  Expansion of 1/(1 - 7*x + 2*x^2).
 * https://oeis.org/A186446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 2*a(n-2) ; a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    seq_t prevprev = (seq_t)1;
    seq_t prev = (seq_t)7;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mul7 = seq_mul(prev, (seq_t)7);
        seq_t mul2 = seq_mul(prevprev, (seq_t)2);
        seq_t cur = mul7 - mul2;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
