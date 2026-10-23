/*
 * A179416  a(n)=1 if (n modulo 65536)+1 is a quadratic residue of 65537, 0
 *          otherwise.
 * https://oeis.org/A179416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (n modulo 65536)+1 is a quadratic residue of 65537,
 *            0 otherwise
 * Range:     u8 n <= 9414, u16 n <= 9414, u32 n <= 9414, u64 n <= 9414,
 *            u128 n <= 9414, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the set of quadratic residues modulo 65537.
       65537 is prime, and its residues form an array of size 65537
       (index 0 unused). The table is built on the first call only. */
    static bool residues[65537] = { false }; /* all false initially */
    static bool initialized = false;

    if (!initialized) {
        const uint64_t mod = 65537ULL;
        for (uint64_t i = 1; i < mod; ++i) {
            uint64_t r = mod_mul(i, i, mod);   /* r is i^2 mod 65537 */
            residues[r] = true;
        }
        initialized = true;
    }

    /* Map index n to the sequence definition */
    uint64_t k = (n % 65536ULL) + 1;   /* k ranges from 1 to 65536 */
    return residues[k] ? (seq_t)1 : (seq_t)0;
}
