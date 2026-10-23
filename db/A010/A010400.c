/*
 * A010400  Squares mod 39.
 * https://oeis.org/A010400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Sequence: a(n) is the n‑th smallest integer that is a quadratic residue
   modulo 39.  The set of residues is {0,1,3,4,9,10,12,13,16,22,25,27,30,36}. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t residues[39];
    static size_t count = 0;

    if (!initialized) {
        bool seen[39] = {0};
        for (idx_t i = 0; i < 39; ++i) {
            idx_t r = (i * i) % 39;
            seen[r] = true;
        }
        for (idx_t i = 0; i < 39; ++i) {
            if (seen[i]) {
                residues[count++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return 0; /* undefined for offset 1, defensive fallback */
    }
    if (n <= count) {
        return residues[n - 1];
    }
    /* Sequence is defined only for n = 1..14; for larger n we return the
       last known residue as a safe default. */
    return residues[count - 1];
}
