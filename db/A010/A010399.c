/*
 * A010399  Squares mod 38.
 * https://oeis.org/A010399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010399: a(n) is the n-th quadratic residue modulo 38
   in increasing order (0,1,4,5,6,7,9,11,16,17,19,20,23,24,25,26,28,30,35,36). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state to generate and cache the residues */
    static bool initialized = false;
    static seq_t residues[38];
    static size_t count = 0;
    static bool seen[38] = {0};

    if (!initialized) {
        /* generate the set of residues */
        const idx_t MOD = 38;
        for (idx_t k = 0; k < MOD; ++k) {
            seq_t sq = (seq_t)k * (seq_t)k % (seq_t)MOD;
            size_t idx = (size_t)sq;
            if (!seen[idx]) {
                seen[idx] = true;
                residues[count++] = sq;
            }
        }
        /* sort the residues ascending */
        for (size_t i = 1; i < count; ++i) {
            for (size_t j = i; j > 0 && residues[j-1] > residues[j]; --j) {
                seq_t tmp = residues[j];
                residues[j] = residues[j-1];
                residues[j-1] = tmp;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return 0;
    }
    if ((size_t)n > count) {
        return 0;
    }
    return residues[(size_t)(n-1)];
}
