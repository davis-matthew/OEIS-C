/*
 * A010375  Squares mod 11.
 * https://oeis.org/A010375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = {0, 1, 3, 4, 5, 9} repeated every 6 terms (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t residues[6] = {0, 1, 3, 4, 5, 9};
    idx_t idx = (n - 1) % 6UL;          /* 0‑based index into residues */
    return residues[(size_t)idx];
}
