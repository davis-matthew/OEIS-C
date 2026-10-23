/*
 * A010163  Continued fraction for sqrt(92).
 * https://oeis.org/A010163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9317, u16 n <= 9317, u32 n <= 9317, u64 n <= 9317,
 *            u128 n <= 9317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(92) – a(0)=9, then period 8: 1,1,2,4,2,1,1,18 */
#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0) return 9;
    static const seq_t pat[8] = {1,1,2,4,2,1,1,18};
    return pat[(size_t)((n - 1) % 8)];
}
