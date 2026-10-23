/*
 * A156702  Numbers k such that k^2 - 1 == 0 (mod 24^2).
 * https://oeis.org/A156702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 288 * floor((n-1)/4) + r[(n-1) % 4], where r[0] = 1, r[1]
 *            = 127, r[2] = 161, r[3] = 287
 * Range:     u8 n <= 3, u16 n <= 910, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static const seq_t residues[4] = {1, 127, 161, 287};
    idx_t q = (n - 1) / 4;                  /* integer division */
    int r = (int)((n - 1) % 4);
    return (seq_t)q * (seq_t)288 + residues[r];
}
