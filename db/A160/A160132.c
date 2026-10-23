/*
 * A160132  Numerator of Hermite(n, 10/27).
 * https://oeis.org/A160132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 20*a(n-1) - 1458*(n-1)*a(n-2), a(0)=1, a(1)=20
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, not required but harmless */

seq_t a(idx_t n)
{
    /* Handle the two initial values explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)20;

    /* Iteratively compute a(n) using the recurrence. */
    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)20;         /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 20*prev1 - 1458*(i-1)*prev2 */
        seq_t term1 = (seq_t)20 * prev1;
        seq_t term2 = (seq_t)1458 * (seq_t)(i - 1) * prev2;
        cur = term1 - term2;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
