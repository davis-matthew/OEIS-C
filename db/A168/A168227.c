/*
 * A168227  a(n) = n^4*(n^8 + 1)/2.
 * https://oeis.org/A168227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1721,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* Build using arbitrary‑precision integer class. */
    seq_t sn = n;
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;
    seq_t n8 = n4 * n4;
    return n4 * (n8 + 1) / 2;
#else
    /* Build using integral types – use 128‑bit intermediate to avoid
       overflow while the result fits in seq_t. */
    unsigned __int128 t = n;          /* n^1 */
    t = t * t;                       /* n^2 */
    t = t * t;                       /* n^4 */
    unsigned __int128 n4 = t;
    t = t * t;                       /* n^8 */
    t = n4 * (t + 1);                /* n^4*(n^8+1) */
    t /= 2;                          /* final division */
    return (seq_t)t;
#endif
}
