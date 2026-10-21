/*
 * A223581  Number of nX4 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 130 * a(n-1) for n > 3 Initial values: a(1) = 256 a(2) =
 *            65536 a(3) = 10240000 a(4) = 1331200000
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset */
    if (n == 0)
        return 0;

    /* Pre‑computed first four terms */
    if (n == 1)
        return (seq_t)256;
    if (n == 2)
        return (seq_t)65536;
    if (n == 3)
        return (seq_t)10240000;
    if (n == 4)
        return (seq_t)1331200000;

    /* For n > 4 use the closed form a(n) = a(4) * 130^(n-4) */
    seq_t pow = big_pow((seq_t)130, (seq_t)(n - 4));
    return (seq_t)1331200000 * pow;
}
