/*
 * A010413  Squares mod 52.
 * https://oeis.org/A010413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = sorted quadratic residues modulo 52, repeated
 *            periodically
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t residues[14];
    static bool initialized = false;

    if (!initialized) {
        bool seen[52] = { false };
        for (idx_t k = 0; k < 52; ++k) {
            idx_t sq = (k * k) % 52;
            seen[(size_t)sq] = true;
        }
        size_t pos = 0;
        for (size_t r = 0; r < 52; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return 0;
    }
    return residues[(size_t)((n - 1) % 14)];
}
