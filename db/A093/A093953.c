/*
 * A093953  a(n) = rightmost term in M^n * [1,1,1], where M = a 3 X 3 matrix
 *          composed of the first 3 rows of A050166 (fill in the matrix with
 *          zeros): = [1 0 0 / 1 2 0 / 1 4 5].
 * https://oeis.org/A093953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 17*a(n-2) + 10*a(n-3), initial values: a(0)=1,
 *            a(1)=10, a(2)=63 This implements OEIS sequence A093953
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, needed by typedefs */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)63;

    /* Recurrence variables: a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)10;
    seq_t a2 = (seq_t)63;
    seq_t a3;

    /* Compute iteratively up to the requested n */
    for (idx_t i = 3; i <= n; ++i) {
        /* a3 = 8*a2 - 17*a1 + 10*a0 */
        a3 = (seq_t)8 * a2 - (seq_t)17 * a1 + (seq_t)10 * a0;
        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
