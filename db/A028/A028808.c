/*
 * A028808  Nonsquares mod 95.
 * https://oeis.org/A028808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = floor((n-1)/65)*95 + r_{(n-1)%65} where r_k are the 65
 *            non‑square residues modulo 95 in ascending order
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* List of residues r (1 ≤ r < 95) such that r is NOT a quadratic residue mod 95. */
    static bool initialized = false;
    static uint8_t residues[65];

    if (!initialized) {
        initialized = true;
        unsigned count = 0;
        for (unsigned r = 1; r < 95; ++r) {
            unsigned r5  = r % 5;
            bool qr5 = (r5 == 0 || r5 == 1 || r5 == 4);          /* residues mod 5 */

            unsigned r19 = r % 19;
            bool qr19 = (r19 == 0 || r19 == 1 || r19 == 4 ||
                         r19 == 5 || r19 == 6 || r19 == 7 ||
                         r19 == 9 || r19 == 11 || r19 == 16 || r19 == 17); /* residues mod 19 */

            if (!(qr5 && qr19)) {            /* not a quadratic residue mod 95 */
                residues[count++] = (uint8_t)r;
            }
        }
    }

    idx_t block   = (n - 1) / 65;        /* how many full blocks of 95 */
    idx_t offset  = (n - 1) % 65;        /* index within the block */

    seq_t value = (seq_t)block * 95 + residues[offset];
    return value;
}
