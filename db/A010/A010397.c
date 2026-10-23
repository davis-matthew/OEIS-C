/*
 * A010397  Squares mod 36.
 * https://oeis.org/A010397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 36 * floor((n-1)/8) + r[(n-1) mod 8] where r = {0, 1, 4,
 *            9, 13, 16, 25, 28}
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Quadratic residues modulo 36, in ascending order. */
    static const seq_t residues[8] = {
        (seq_t)0,  (seq_t)1,  (seq_t)4,  (seq_t)9,
        (seq_t)13, (seq_t)16, (seq_t)25, (seq_t)28
    };

    /* n >= 1; compute block index and residue index. */
    idx_t block = (n - 1) / 8;
    idx_t r_index = (n - 1) % 8;

    /* Compute the value: 36 * block + residues[r_index]. */
    seq_t result = ((seq_t)36) * (seq_t)block + residues[(size_t)r_index];
    return result;
}
