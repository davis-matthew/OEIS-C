/*
 * A056091  Numbers k such that k^4 == 1 (mod 5^4).
 * https://oeis.org/A056091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 625*⌊(n-1)/4⌋ + {1,182,443,624}[(n-1) mod 4]
 * Range:     u8 n <= 2, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t residue[4] = { (seq_t)1, (seq_t)182, (seq_t)443, (seq_t)624 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1, safe return */
    idx_t zero  = n - 1;                  /* zero‑based index */
    idx_t block = zero / 4;                /* number of complete 625‑blocks */
    idx_t pos   = zero % 4;                /* position inside the block */
    return residue[pos] + (seq_t)625 * (seq_t)block;
}
