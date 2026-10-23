/*
 * A177755  Number of ways to place 2 nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A177755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/4 * n^2 * (2*n^2 - 4*n + 3 + (-1)^n)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 297, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ways to place 2 nonattacking bishops on an n×n toroidal board. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 for safety. */
    if (n == 0)
        return 0;

    /* n^2 */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* term = 2*n^2 - 4*n + 3 + (-1)^n */
    seq_t term =  (seq_t)2 * nn
                - (seq_t)4 * n
                + (seq_t)3;

    if ((n & 1) == 0)   /* even n: add +1 */
        term += (seq_t)1;
    else                /* odd n: add -1 */
        term -= (seq_t)1;

    /* answer = nn * term / 4  */
    seq_t ans = nn * term;
    ans /= (seq_t)4;   /* division is exact for all n ≥ 1 */

    return ans;
}
