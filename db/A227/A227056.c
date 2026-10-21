/*
 * A227056  Number of n X 2 -2..2 arrays of 2 X 2 subblock diagonal sums minus
 *          antidiagonal sums for some (n+1) X 3 binary array with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5040)*n^7 + (1/180)*n^6 + (23/360)*n^5 + (25/72)*n^4 *
 *            + (907/720)*n^3 + (1133/360)*n^2 + (877/210)*n + 1
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */

    unsigned __int128 N = (unsigned __int128)n;

    /* Compute powers of n up to n^7 */
    unsigned __int128 n1 = N;       /* n^1 */
    unsigned __int128 n2 = n1 * N;  /* n^2 */
    unsigned __int128 n3 = n2 * N;  /* n^3 */
    unsigned __int128 n4 = n3 * N;  /* n^4 */
    unsigned __int128 n5 = n4 * N;  /* n^5 */
    unsigned __int128 n6 = n5 * N;  /* n^6 */
    unsigned __int128 n7 = n6 * N;  /* n^7 */

    /* Assemble the numerator of the fraction, which is guaranteed to be divisible by 5040 */
    unsigned __int128 numer = n7;
    numer += (unsigned __int128)28   * n6;
    numer += (unsigned __int128)322  * n5;
    numer += (unsigned __int128)1750 * n4;
    numer += (unsigned __int128)6349 * n3;
    numer += (unsigned __int128)15862 * n2;
    numer += (unsigned __int128)21048 * n1;
    numer += (unsigned __int128)5040;

    return (seq_t)(numer / 5040);
}
