/*
 * A129792  a(n) = ceiling(8^n/n).
 * https://oeis.org/A129792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(8^n / n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined for n >= 1 by OEIS */

    /* If seq_t is 64‑bit (or smaller), use 128‑bit arithmetic to avoid overflow.
       For larger seq_t types (e.g. arbitrary‑precision) fall back to direct
       multiplications in the seq_t type. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate */
        unsigned __int128 pow = (unsigned __int128)1;
        for (idx_t i = 0; i < n; ++i) pow *= 8;

        unsigned __int128 denom = (unsigned __int128)n;
        unsigned __int128 res  = (pow + denom - 1) / denom;

        return (seq_t)res;
    } else {
        /* seq_t has enough capacity (big integer) */
        seq_t pow = (seq_t)1;
        for (idx_t i = 0; i < n; ++i) pow = pow * (seq_t)8;

        seq_t denom = (seq_t)n;
        seq_t res   = pow / denom;
        if (pow % denom != 0) ++res;
        return res;
    }
}
