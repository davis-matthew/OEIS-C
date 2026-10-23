/*
 * A034131  a(n) = ceiling((n + 1/2)^3).
 * https://oeis.org/A034131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/2)^3)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset 1: n >= 1 */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 w = 2 * N + 1;                /* w = 2n+1 */
    unsigned __int128 c = w * w * w;               /* w^3   */
    c += 7;                                        /* w^3 + 7   (for ceil division by 8) */
    c >>= 3;                                       /* divide by 8 */
    return (seq_t)c;
}
