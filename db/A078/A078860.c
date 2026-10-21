/*
 * A078860  Least positive residues [mod 210] representing those residue
 *          classes which can be lesser of prime pairs from A029710.
 * https://oeis.org/A078860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth least positive residue r (mod 210) such that
 *            gcd(r,210)=1 and gcd((r+4) mod 210,210)=1, with the special case
 *            r=7 included even though gcd(7,210)!=1. The sequence has exactly
 *            16 such residues:
 *            7,13,19,37,43,67,79,97,103,109,127,139,163,169,187,193
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_int.h"

seq_t a(idx_t n) {
    static seq_t residues[16];
    static size_t count = 0;
    if (count == 0) {
        /* Compute residues satisfying the condition */
        for (int r = 0; r < 210; ++r) {
            if (r == 7) {
                residues[count++] = 7;
                continue;
            }
            if (gcd_int(r, 210) == 1) {
                int r4 = (r + 4) % 210;
                if (gcd_int(r4, 210) == 1) {
                    residues[count++] = (seq_t)r;
                }
            }
        }
    }
    if (n == 0 || n > count) {
        return (seq_t)0;      /* Undefined or out of range */
    }
    return residues[n - 1];
}
