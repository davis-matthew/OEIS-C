/*
 * A172141  Number of ways to place 2 nonattacking nightriders on an n X n
 *          board.
 * https://oeis.org/A172141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n^3 - 5*n^2 + 9*n - 4)/6 if n is even, a(n) = n*(n -
 *            1)*(3*n^2 - 2*n + 7)/6 if n is odd
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 248, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset starts at 1; safe fallback */

    seq_t N = (seq_t)n;

    if ((n & 1) == 0) {         /* even n */
        seq_t term = N * ( (seq_t)3 * N * N * N
                          - (seq_t)5 * N * N
                          + (seq_t)9 * N
                          - (seq_t)4 );
        return term / 6;
    } else {                    /* odd n */
        seq_t term = N * (N - 1) * ( (seq_t)3 * N * N
                                     - (seq_t)2 * N
                                     + (seq_t)7 );
        return term / 6;
    }
}
