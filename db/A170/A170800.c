/*
 * A170800  a(n) = n^10*(n^8 + 1)/2.
 * https://oeis.org/A170800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 12, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate arithmetic for 64‑bit seq_t */
        typedef unsigned __int128 u128;
        u128 y = (u128)n;
        u128 n2 = y * y;
        u128 n4 = n2 * n2;
        u128 n8 = n4 * n4;
        u128 n10 = n8 * n2;
        u128 res = n10 * (n8 + 1) / 2;
        return (seq_t)res;
    }
#endif
    /* For arbitrary‑precision seq_t or when 128‑bit arithmetic is unavailable */
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;
    seq_t n4 = n2 * n2;
    seq_t n8 = n4 * n4;
    seq_t n10 = n8 * n2;
    seq_t res = n10 * (n8 + 1) / 2;
    return res;
}
