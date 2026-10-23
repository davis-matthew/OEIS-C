/*
 * A047516  Numbers that are congruent to {1, 3, 4, 6, 7} mod 8.
 * https://oeis.org/A047516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest positive integer congruent to {1,3,4,6,7} (mod
 *            8) that is the nth such integer
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Each block of 8 consecutive integers contains exactly 5 allowed
       residues: 1,3,4,6,7.
       For n ≥ 1 let:
         q = (n‑1)/5   – number of complete blocks before the desired
         r = (n‑1)%5   – position within the current block (0…4)
       Then the value is q*8 + allowed[r]. */
    static const uint8_t allowed[5] = {1, 3, 4, 6, 7};
    idx_t q = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    return (seq_t)(q * 8 + allowed[r]);
}
