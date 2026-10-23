/*
 * A010421  Squares mod 60.
 * https://oeis.org/A010421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Sequence: a(n) cycles through the 12 distinct residues of squares modulo 60.
   a(n) = result[(n-1) mod 12]  where the 12 residues are
   0,1,4,9,16,21,24,25,36,40,45,49. */
seq_t a(idx_t n)
{
    static seq_t res[12];
    static bool ready = false;

    if (!ready) {
        bool seen[60] = { false };
        for (idx_t k = 0; k < 60; ++k) {
            idx_t sq = (k * k) % 60;
            seen[(size_t)sq] = true;
        }
        idx_t pos = 0;
        for (idx_t v = 0; v < 60; ++v) {
            if (seen[(size_t)v]) {
                res[pos++] = (seq_t)v;
            }
        }
        ready = true;
    }

    idx_t idx = (n - 1) % 12;
    return res[(size_t)idx];
}
