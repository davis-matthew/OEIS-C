/*
 * A157062  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 26.
 * https://oeis.org/A157062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..min(n,13)} C(n+1,i) * C(12,i-1) * C(n-i+13,13)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 29, u128 n <= 171,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                     (OEIS A157062) */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t max_i = n;
    if (max_i > 13)
        max_i = 13;

    seq_t total = 0;
    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t term = binom(n + 1, i);
        term *= binom(12, i - 1);
        term *= binom(n - i + 13, 13);
        total += term;
    }
    return total;
}
