/*
 * A194933  Number of lower triangles of a 4 X 4 0..n array with no element
 *          differing from any of its horizontal or vertical neighbors by more
 *          than one.
 * https://oeis.org/A194933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=6939*n-8379 (for n>4), a(1)=1024, a(2)=5873, a(3)=12474,
 *            a(4)=19379
 * Range:     u8 none, u16 n <= 10, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Exact values for the first four indices */
    static const seq_t init[4] = {1024, 5873, 12474, 19379};

    if (n <= 4)
        return init[n - 1];

    /* For n>=5: a(n) = 6939*n - 8379 */
    return (seq_t)6939 * (seq_t)n - (seq_t)8379;
}
