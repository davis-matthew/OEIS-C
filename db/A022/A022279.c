/*
 * A022279  a(n) = n*(21*n + 1)/2.
 * https://oeis.org/A022279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(21*n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 78, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow in the intermediate products. */
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 v = x * (21 * x + 1) / 2;
    return (seq_t)v;
}
