/*
 * A028739  Nonsquares mod 26.
 * https://oeis.org/A028739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) equals the n-th positive integer that is NOT a quadratic
 *            residue modulo 26. Equivalently, a(n) = 26 * floor((n-1)/12) +
 *            s, where s is the ((n-1) mod 12)-th element of
 *            {2,5,6,7,8,11,15,18,19,20,21,24}
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t unsquare_residues[12] =
{
    2, 5, 6, 7, 8, 11,
   15,18,19,20,21,24
};

seq_t a(idx_t n)
{
    /* n is offset 1, so a(1) is the first term */
    idx_t block = (n - 1) / 12;           /* 0‑based block index */
    idx_t pos   = (n - 1) % 12;           /* position inside the block */
    return (seq_t)26 * (seq_t)block + (seq_t)unsquare_residues[pos];
}
