/*
 * A165679  Seventh right hand column of triangle A165674.
 * https://oeis.org/A165679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*n^6 + 126*n^5 + 875*n^4 + 2940*n^3 + 4872*n^2 + 3528*n
 *            + 720
 * Range:     u8 none, u16 n <= 1, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use Horner's method with 128‑bit intermediate to avoid overflow. */
    __int128 x = n;     /* promote index to 128‑bit */
    __int128 r = 7;
    r = r * x + 126;
    r = r * x + 875;
    r = r * x + 2940;
    r = r * x + 4872;
    r = r * x + 3528;
    r = r * x + 720;
    return (seq_t)r;
}
