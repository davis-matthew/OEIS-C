/*
 * A219699  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..1 n X 3 array.
 * https://oeis.org/A219699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2n^3 – 25n^2 + 214n – 240) / 24 for n > 1, a(1) =
 *            3
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    long long nn = (long long)n;         /* safe: n ≤ 10000 for performance test */
    long long n2  = nn * nn;             /* n² */
    long long n3  = n2 * nn;             /* n³ */
    long long n4  = n2 * n2;             /* n⁴ */

    long long num = n4 + 2 * n3 - 25 * n2 + 214 * nn - 240;
    return (seq_t)(num / 24);
}
