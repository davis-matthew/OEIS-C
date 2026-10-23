/*
 * A010394  Squares mod 33.
 * https://oeis.org/A010394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   list of distinct square residues modulo 33, sorted in increasing
 *            order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t residues[12];
    static bool init = false;

    if (!init) {
        bool seen[33] = { false };
        for (idx_t i = 0; i < 33; ++i) {
            seq_t r = ((seq_t)i * (seq_t)i) % 33;
            seen[(unsigned int)r] = true;
        }

        idx_t pos = 0;
        for (idx_t v = 0; v < 33; ++v) {
            if (seen[(unsigned int)v]) {
                residues[pos++] = (seq_t)v;
            }
        }
        init = true;
    }

    return residues[(n - 1) % 12];
}
