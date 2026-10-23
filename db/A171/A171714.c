/*
 * A171714  a(n) = ceiling((n+1)^4/2).
 * https://oeis.org/A171714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((n+1)^4/2)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 302, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 1;   /* m = n+1 */
    m = m * m;                /* m = (n+1)^2 */
    m = m * m;                /* m = (n+1)^4 */
    return (m + 1) / 2;       /* ceil(m/2) */
}
