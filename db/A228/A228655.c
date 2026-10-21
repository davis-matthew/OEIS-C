/*
 * A228655  Number of nX3 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 3*a(n-3) for n ≥ 4 * Initial
 *            values: a(1) = 2, a(2) = 8, a(3) = 34 * The function returns the
 *            required term for any n ≥ 1, * performing the computation in
 *            O(n) time with a constant space usage
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed in the build */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == (idx_t)1)
        return (seq_t)2;
    if (n == (idx_t)2)
        return (seq_t)8;
    if (n == (idx_t)3)
        return (seq_t)34;

    /* t1 = a(n-3), t2 = a(n-2), t3 = a(n-1) */
    seq_t t1 = (seq_t)2;
    seq_t t2 = (seq_t)8;
    seq_t t3 = (seq_t)34;

    /* Compute forward from n = 4 up to the requested n. */
    for (idx_t i = (idx_t)4; i <= n; ++i) {
        seq_t next = 5 * t3 - 3 * t2 - 3 * t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return t3;
}
