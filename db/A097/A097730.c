/*
 * A097730  Pell equation solutions (6*b(n))^2 - 37*a(n)^2 = -1 with b(n) =
 *          A097729(n), n >= 0.
 * https://oeis.org/A097730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 145, a(n) = 146*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)145;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)145; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)146 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
