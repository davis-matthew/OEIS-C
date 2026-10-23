/*
 * A165187  a(n) = n^3*(n+1)^2*(n+2)/12.
 * https://oeis.org/A165187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n+1)^2*(n+2)/12
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n1  = nn + 1;
    seq_t n2  = nn + 2;

    /* Factors: n, n, n, n+1, n+1, n+2 */
    seq_t f[6] = {nn, nn, nn, n1, n1, n2};

    /* Number of factors of 2 and 3 that must be removed
       (12 = 2^2 * 3) */
    int twos   = 2;
    int threes = 1;

    seq_t prod = 1;

    for (int i = 0; i < 6; ++i) {
        seq_t v = f[i];

        /* Remove factors of 2 */
        while (twos > 0 && (v & 1) == 0) {
            v >>= 1;
            twos--;
        }

        /* Remove factors of 3 */
        while (threes > 0 && v % 3 == 0) {
            v /= 3;
            threes--;
        }

        prod *= v;
    }

    return prod;
}
