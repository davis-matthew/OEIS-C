/*
 * A056088  Numbers k such that k^16 == 1 (mod 17^3).
 * https://oeis.org/A056088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that k^16 ≡ 1 (mod 4913).
   The set of solutions modulo 4913 consists of 16 residues; the sequence
   lists all integers congruent to any of those residues, ordered
   ascending.  Hence:

   Let residues[0..15] be the 16 residues (k modulo 4913) with k^16 ≡ 1.
   Then for n ≥ 1:
       group = (n-1) / 16
       pos   = (n-1) % 16
       a(n)  = residues[pos] + group * 4913
*/
#include "../utils/_mod_pow.h"

seq_t a(idx_t n)
{
    /* Precompute the 16 residues modulo 4913 exactly once. */
    static seq_t residues[16];
    static bool done = false;
    if (!done) {
        idx_t count = 0;
        for (seq_t x = 1; x < (seq_t)4913; ++x) {
            if (_mod_pow(x, 16, (seq_t)4913) == (seq_t)1) {
                residues[count++] = x;
                if (count == 16) break;
            }
        }
        done = true;
    }

    idx_t idx    = n - 1;           /* zero‑based index */
    idx_t group  = idx / 16;        /* how many full blocks of 4913 */
    idx_t pos    = idx % 16;        /* position within the 16 residues */

    return residues[pos] + (seq_t)group * (seq_t)4913;
}
