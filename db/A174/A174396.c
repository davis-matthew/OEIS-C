/*
 * A174396  Numbers congruent to {1,4,5,8} mod 9.
 * https://oeis.org/A174396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*floor((n-1)/4) + {1,4,5,8}[ (n-1)%4 ]
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t k = (n - 1) / 4;               /* floor((n-1)/4) */
    idx_t r = (n - 1) % 4;               /* (n-1) mod 4 */

    static const seq_t residues[4] = {1, 4, 5, 8};

    seq_t base = (seq_t)9 * (seq_t)k;
    return base + residues[(size_t)r];
}
