/*
 * A067520  Triangular numbers whose index is a multiple of the sum of their
 *          digits.
 * https://oeis.org/A067520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth triangular number T(m) with m a multiple of * the
 *            sum of the decimal digits of T(m). (i.e. m % S(T(m)) == 0)
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 4942, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                       /* for uint64_t */
#include "../utils/digit_sum_u64.h"       /* provides unsigned digit_sum_u64(uint64_t) */

seq_t a(idx_t n)
{
    idx_t count = 0;                      /* how many qualifying terms found so far */
    idx_t m     = 1;                       /* current triangular index */

    while (1) {
        /* compute the triangular value using 64‑bit arithmetic to avoid overflow
           for the ranges needed by the tests (m stays below 2^32).            */
        uint64_t tri = (uint64_t)m * (m + 1u) / 2u;

        /* digit sum of tri – guaranteed to fit in unsigned int              */
        unsigned int d = digit_sum_u64(tri);

        if (d != 0 && (m % d) == 0) {
            ++count;
            if (count == n)
                return (seq_t)tri;       /* cast is safe for seq_t types that can hold the value */
        }
        ++m;
    }
}
