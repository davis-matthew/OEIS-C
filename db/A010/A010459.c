/*
 * A010459  Squares mod 98.
 * https://oeis.org/A010459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th smallest quadratic residue modulo 98 (sorted ascending) */

#include <stddef.h>
#include <stdbool.h>

#define MODULO 98

seq_t a(idx_t n) {
    /* Static table of quadratic residues modulo 98, sorted ascending. */
    static seq_t sorted_residues[MODULO];
    static size_t sorted_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Compute residues i^2 mod 98 for i = 0 .. 97. */
        bool seen[MODULO] = { false };
        for (idx_t i = 0; i < MODULO; ++i) {
            /* Convert i to seq_t before arithmetic. */
            seq_t s = ((seq_t)i * ((seq_t)i + 1) / 2); /* dummy to use seq_t arithmetic */
            /* Actually we need i*i mod MODULO */
            /* Use seq_t multiplication but avoid overflow by casting to seq_t. */
            seq_t sq = ((seq_t)i * ((seq_t)i)) % (seq_t)MODULO;
            seen[(size_t)sq] = true;
        }
        /* Fill sorted array */
        size_t idx = 0;
        for (size_t r = 0; r < MODULO; ++r) {
            if (seen[r]) {
                sorted_residues[idx++] = (seq_t)r;
            }
        }
        sorted_len = idx;
        initialized = true;
    }

    if (n == 0 || (idx_t)(n - 1) >= sorted_len) {
        /* Undefined terms after the list ends; return 0 as safe default. */
        return (seq_t)0;
    }
    return sorted_residues[(size_t)(n - 1)];
}
