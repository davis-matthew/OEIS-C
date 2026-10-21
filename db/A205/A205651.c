/*
 * A205651  Period 6: repeat [1, 6, 5, 4, 9, 0].
 * https://oeis.org/A205651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n>6, with initial values a(1)=1, a(2)=6,
 *            a(3)=5, a(4)=4, a(5)=9, a(6)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period-6 pattern starting at index 1 */
    static const seq_t pattern[6] = {1, 6, 5, 4, 9, 0};
    idx_t idx = (n - 1) % 6;   /* zero‑based remainder in 0..5 */
    return pattern[(int)idx];
}
