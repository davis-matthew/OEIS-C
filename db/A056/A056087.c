/*
 * A056087  Numbers k such that k^14 == 1 (mod 15^3).
 * https://oeis.org/A056087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056087:  k such that k^14 ≡ 1 (mod 15^3 = 3375).
    The residues modulo 3375 that satisfy this congruence are
    1, 1376, 1999, 3374.  All terms of the sequence are
    obtained by taking these residues and adding multiples
    of 3375 in increasing order.
    Hence for n≥1:
        a(n) = ⌊(n‑1)/4⌋·3375 + R[(n‑1) mod 4]
    where R[0] = 1, R[1] = 1376, R[2] = 1999, R[3] = 3374.
*/

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* modulus 15^3 */
    const int MOD = 3375;

    /* residues r (0 < r < MOD) with r^14 ≡ 1 (mod MOD) */
    static const int residues[4] = {1, 1376, 1999, 3374};

    idx_t q = (n - 1) / 4;
    idx_t i = (n - 1) % 4;

    return (seq_t)((uint64_t)q * (uint64_t)MOD + (uint64_t)residues[i]);
}
