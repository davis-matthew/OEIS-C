/*
 * A235537  Expansion of (6 + 13*x - 8*x^2 - 8*x^3 + 6*x^4)/((1 + x)^2*(1 -
 *          x)^3).
 * https://oeis.org/A235537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(3*n + 17) - (2*n + 43)*(-1)^n + 11)/16 + 8
 *            (equivalent unconditional versions: even n: a(n) = (18*n*n +
 *            100*n - 32)/16 + 8 odd n: a(n) = (18*n*n + 104*n + 54)/16 + 8 )
 * Range:     u8 n <= 12, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use signed 64‑bit arithmetic for the intermediate
     * (n is at most 10 000 in the tests, so no overflow). */
    int64_t N = (int64_t)n;
    int64_t num;

    if ((n & 1ULL) == 0ULL) {                /* n even */
        num = 18 * N * N + 100 * N - 32;
    } else {                                  /* n odd */
        num = 18 * N * N + 104 * N + 54;
    }

    /* num is guaranteed to be divisible by 16 */
    int64_t div = num / 16;
    int64_t result = div + 8;

    return (seq_t)result;
}
