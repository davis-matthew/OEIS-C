/*
 * A033540  a(n+1) = n*(a(n) + 1) for n >= 1, a(1) = 1.
 * https://oeis.org/A033540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n+1) = n * (a(n) + 1) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;              /* a(1) */
    for (idx_t i = 1; i < n; ++i) {    /* compute up to a(n) */
        val = (seq_t)i * (val + (seq_t)1);
    }
    return val;
}
