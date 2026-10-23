/*
 * A108928  a(n) = 8*n^2 - 3.
 * https://oeis.org/A108928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n^2 - 3
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid intermediate overflow
       for any n that yields a result that fits in seq_t */
    unsigned __int128 tmp = (unsigned __int128)n * n;  // n^2
    tmp = tmp * 8 - 3;                                 // 8*n^2 - 3
    return (seq_t)tmp;
}
