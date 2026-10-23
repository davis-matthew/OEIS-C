/*
 * A108034  n!*(9*n^4-78*n^3+235*n^2-438*n+680)/72.
 * https://oeis.org/A108034
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(9*n^4-78*n^3+235*n^2-438*n+680)/72
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* defined only for n ≥ 4 by OEIS */

    /* polynomial part: 9*n^4 - 78*n^3 + 235*n^2 - 438*n + 680 */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2   */
    seq_t n3 = n2 * N;         /* n^3   */
    seq_t n4 = n2 * n2;        /* n^4   */

    seq_t poly = 9 * n4 - 78 * n3 + 235 * n2 - 438 * N + 680;

    /* factorial part */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* cancel the division by 72 as early as possible to avoid overflow */
    if (n >= 9) {              /* n! is divisible by 72 for n ≥ 9 */
        fact /= 72;
    }

    /* multiply polynomial with the (fractionally) reduced factorial */
    seq_t result = fact * poly;

    /* for n < 9 the division by 72 was postponed until now */
    if (n < 9)
        result /= 72;

    return result;
}
