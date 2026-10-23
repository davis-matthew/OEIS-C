/*
 * A010409  Squares mod 48.
 * https://oeis.org/A010409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = [0,1,4,9,16,25,33,36][(n-1) mod 8] (distinct squares
 *            modulo 48, sorted)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t residues[8];
    static bool initialized = false;

    if (!initialized) {
        /* Determine which residues modulo 48 are quadratic residues. */
        uint8_t used[48] = {0};

        for (idx_t k = 0; k < 48; ++k) {
            seq_t r = ((seq_t)k * (seq_t)k) % (seq_t)48;
            used[(unsigned)r] = 1;
        }

        /* Collect them in ascending order. */
        size_t pos = 0;
        for (int r = 0; r < 48; ++r) {
            if (used[r]) {
                residues[pos++] = (seq_t)r;
            }
        }

        initialized = true;
    }

    size_t idx = (size_t)((n - 1) % 8);
    return residues[idx];
}
