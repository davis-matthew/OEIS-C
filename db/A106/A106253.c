/*
 * A106253  First difference of A106252.
 * https://oeis.org/A106253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) cycles with period 6: 2,2,2,1,3,1 (a(1)=2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {2, 2, 2, 1, 3, 1};
    idx_t pos = (n - 1U) % 6U;          /* position in the period, 0‑based */
    return pattern[(size_t)pos];
}
