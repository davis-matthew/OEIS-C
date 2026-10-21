/*
 * A069908  Numbers congruent to +-2, +-3, +-4 or +-5 mod 16.
 * https://oeis.org/A069908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = residues[(n-1) mod 8] + 16 * floor((n-1)/8) where
 *            residues = {2,3,4,5,11,12,13,14}
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const idx_t residues[8] = {2,3,4,5,11,12,13,14};
    /* One‑based index n, offset 1 */
    idx_t q = (n - 1) / 8;          /* number of complete 16‑block groups   */
    idx_t r = (n - 1) % 8;          /* position within the current group    */

    /* Compute 16*q without overflow when possible */
#ifdef __SIZEOF_INT128__
    /* GCC/Clang support for 128‑bit integers (C/C++17) */
    return (seq_t)((__uint128_t)16 * q + residues[r]);
#else
    /* Fall back to 64‑bit arithmetic – adequate for typical test ranges */
    return (seq_t)(16 * q + residues[r]);
#endif
}
