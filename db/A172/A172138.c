/*
 * A172138  Number of ways to place 3 nonattacking zebras on an n X n board.
 * https://oeis.org/A172138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 27*n^4 + 120*n^3 + 74*n^2 - 1608*n + 2976)/6 for
 *            n>=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small n values from OEIS data */
    static const seq_t small_vals[6] = {0, 0, 4, 84, 452, 1772};

    if (n < 6)
        return small_vals[n];

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                 /* n^2 */
    seq_t n3 = n2 * nn;                 /* n^3 */
    seq_t n4 = n2 * n2;                 /* n^4 */
    seq_t n6 = n3 * n3;                 /* n^6 */

    seq_t numer = n6
                  - (seq_t)27 * n4
                  + (seq_t)120 * n3
                  + (seq_t)74 * n2
                  - (seq_t)1608 * nn
                  + (seq_t)2976;

    return numer / (seq_t)6;
}
