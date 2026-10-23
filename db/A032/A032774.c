/*
 * A032774  a(n) = floor( n*(n+1)*(n+2)*...*(n+6) / (n+(n+1)+(n+2)+...+(n+6))
 *          ).
 * https://oeis.org/A032774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6) / (7*(n+3))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For 64‑bit (or smaller) seq_t use an unsigned __int128 intermediate
        to avoid overflow; for larger seq_t use ordinary seq_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod = 1;
        for (idx_t i = 0; i < 7; ++i)
            prod *= (unsigned __int128)(n + i);

        unsigned __int128 denom = 7 * (unsigned __int128)(n + 3);
        unsigned __int128 res   = prod / denom;
        return (seq_t)res;
    } else {
        seq_t prod = 1;
        for (idx_t i = 0; i < 7; ++i)
            prod = prod * (seq_t)(n + i);

        seq_t denom = 7 * (seq_t)(n + 3);
        seq_t res   = prod / denom;
        return res;
    }
}
