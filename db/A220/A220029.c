/*
 * A220029  Number of n X 5 arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 5 array.
 * https://oeis.org/A220029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 10*n^3 + 11*n^2 + 122*n - 96)/24 for n>1, a(1) = 5
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)(n ? 5 : 0);    /* a(1)=5, a(0) is not defined */

    const seq_t sn = (seq_t)n;
    const seq_t n2 = sn * sn;          /* n^2 */
    const seq_t n3 = n2 * sn;          /* n^3 */
    const seq_t n4 = n3 * sn;          /* n^4 */

    const seq_t numerator = n4
                           + (seq_t)10 * n3
                           + (seq_t)11 * n2
                           + (seq_t)122 * sn
                           - (seq_t)96;

    return numerator / (seq_t)24;
}
