/*
 * A200979  Number of ways to arrange n books on 4 consecutive bookshelves,
 *          leaving no shelf empty.
 * https://oeis.org/A200979
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*binomial(n-1,3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The function returns 0 for n < 4 (offset behaviour not enforced). */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t bin = (seq_t)(n - 1) * (seq_t)(n - 2) * (seq_t)(n - 3) / 6;

    return fact * bin;
}
