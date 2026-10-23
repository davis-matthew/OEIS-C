/*
 * A123941  The (1,2)-entry in the 3 X 3 matrix M^n, where M = {{2, 1, 1}, {1,
 *          1, 0}, {1, 0, 0}}.
 * https://oeis.org/A123941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 3*a(n-1) - a(n-3) for n ≥ 3 with initial values a(0) = 0,
 *            a(1) = 1, a(2) = 3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* seq_t and idx_t are defined in the build environment */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;

    /* previous three values of the sequence */
    seq_t prev3 = (seq_t)0;          /* a(n-3) */
    seq_t prev2 = (seq_t)1;          /* a(n-2) */
    seq_t prev1 = (seq_t)3;          /* a(n-1) */
    seq_t current = prev1;           /* placeholder for a(n) */

    /* iterate from 3 up to n */
    for (idx_t i = (idx_t)3; i <= n; ++i) {
        current = (prev1 * (seq_t)3) - prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
