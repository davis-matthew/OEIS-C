/*
 * A064202  a(n) = n*(n+1)*(n+2)*(2*n^3 + 6*n^2 + 7*n - 3)/36.
 * https://oeis.org/A064202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(2*n^3 + 6*n^2 + 7*n - 3)/36
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t tn = (seq_t)n;
    seq_t poly = (seq_t)2 * tn * tn * tn
               + (seq_t)6 * tn * tn
               + (seq_t)7 * tn
               - (seq_t)3;

    seq_t factors[4];
    factors[0] = tn;
    factors[1] = tn + 1;
    factors[2] = tn + 2;
    factors[3] = poly;

    /* Small divisors of 36 = 2*2*3*3 */
    const unsigned smallDiv[4] = {2, 2, 3, 3};
    unsigned used[4] = {0};

    seq_t result = 1;
    for (int i = 0; i < 4; ++i) {
        seq_t f = factors[i];
        for (int j = 0; j < 4; ++j) {
            if (!used[j] && f % smallDiv[j] == 0) {
                f /= smallDiv[j];
                used[j] = 1;
            }
        }
        result *= f;
    }

    /* Remaining divisor product */
    seq_t remaining = 1;
    for (int j = 0; j < 4; ++j)
        if (!used[j])
            remaining *= smallDiv[j];

    if (remaining > 1)
        result /= remaining;

    return result;
}
