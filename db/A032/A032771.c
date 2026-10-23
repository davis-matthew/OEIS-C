/*
 * A032771  [ n(n+1)(n+2)...(n+5) / (n+(n+1)+(n+2)+...+(n+5)) ].
 * https://oeis.org/A032771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5) / (n+(n+1)+...+(n+5))
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer division (floor). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Denominator = 6n + 15 = sum of n through n+5 */
    idx_t denom = 6 * n + 15;

#if defined(__SIZEOF_INT128__)
    /* Use 128-bit arithmetic for intermediate product when available */
    unsigned __int128 prod = 1;
    for (idx_t i = 0; i < 6; ++i) {
        prod *= (unsigned __int128)(n + i);
    }
    unsigned __int128 result = prod / (unsigned __int128)denom;
    return (seq_t)result;
#else
    /* Fallback: use the seq_t type directly (big‑int builds will handle
       any overflow automatically). */
    seq_t prod = 1;
    for (idx_t i = 0; i < 6; ++i) {
        prod *= (seq_t)(n + i);
    }
    seq_t result = prod / denom;
    return result;
#endif
}
