/*
 * A193390  The hyper-Wiener index of a benzenoid consisting of a
 *          straight-line chain of n hexagons (s=2; see the Gutman et al.
 *          reference).
 * https://oeis.org/A193390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^4 + 32*n^3 + 46*n^2 + 37*n + 3)/3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 199, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t num = 8ULL * nn * nn * nn * nn
                 + 32ULL * nn * nn * nn
                 + 46ULL * nn * nn
                 + 37ULL * nn
                 + 3ULL;
    num /= 3ULL;
    return (seq_t)num;
}
