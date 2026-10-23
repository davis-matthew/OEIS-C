/*
 * A047380  Numbers that are congruent to {1, 2, 4, 5} mod 7.
 * https://oeis.org/A047380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/4) + {1,2,4,5}[(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */
    idx_t block = (n - 1) / 4;
    size_t rem   = (size_t)((n - 1) % 4);
    static const seq_t residues[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)5 };
    return (seq_t)7 * (seq_t)block + residues[rem];
}
