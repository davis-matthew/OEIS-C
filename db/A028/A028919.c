/*
 * A028919  Congruent to 0, 6, 10, 12 (mod 14).
 * https://oeis.org/A028919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*floor((n-1)/4) + [0,6,10,12]((n-1)%4)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Allowed residues modulo 14 */
    const seq_t pattern[4] = {0, 6, 10, 12};

    /* Convert index to seq_t and shift to 0‑based */
    seq_t t = (seq_t)(n - 1);

    /* Number of full 14‑number blocks before the desired element */
    seq_t block = t / 4;

    /* Position inside the current block (0‑based) */
    seq_t pos   = t % 4;

    return block * 14 + pattern[(size_t)pos];
}
