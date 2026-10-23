/*
 * A010431  Squares mod 70.
 * https://oeis.org/A010431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = sorted distinct quadratic residues modulo 70
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static data for the 24 residues */
    static seq_t residues[70];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        bool seen[70] = { false };
        for (size_t i = 0; i < 70U; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i;      /* i² */
            seq_t r  = sq % (seq_t)70U;          /* residue */
            if (!seen[(size_t)r]) {
                seen[(size_t)r] = true;
                residues[count++] = r;
            }
        }
        /* sort the collected residues (insertion sort, count ≤ 24) */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = residues[i];
            size_t j = i;
            while (j > 0 && residues[j - 1] > key) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }
        initialized = true;
    }

    if (n == 0) return (seq_t)0;                     /* safety for offset */
    if ((size_t)n <= count) return residues[(size_t)n - 1];
    return (seq_t)0;                                 /* no more terms */
}
