/*
 * A108491  Smallest prime with a run of n strictly decreasing digits.
 * https://oeis.org/A108491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = smallest prime with a run of n strictly decreasing
 *            decimal digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precomputed values for 1 ≤ n ≤ 10.
       For n > 10 no such prime exists, so return 0. */
    static const seq_t values[11] = {
        0ULL,               /* dummy for index 0 */
        2ULL,
        31ULL,
        421ULL,
        5431ULL,
        75431ULL,
        764321ULL,
        8764321ULL,
        97654321ULL,
        9765432101ULL,
        198765432101ULL
    };

    if (n < 1 || n > 10)
        return 0;
    return values[n];
}
