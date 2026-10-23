/*
 * A010376  Squares mod 13.
 * https://oeis.org/A010376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010376: a(n) = the list of distinct quadratic residues modulo 13,
   sorted in increasing order and repeated with period 7. */

#include <stdbool.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* static storage for the sorted distinct residues modulo 13 */
    static seq_t residues[13];
    static size_t res_count = 0;      /* number of distinct residues after init */
    static bool init = false;

    if (!init) {
        /* compute all squares modulo 13 for k = 0 .. 12 */
        size_t i = 0;
        for (idx_t k = 0; k < 13; ++k) {
            residues[i++] = (seq_t)k * (seq_t)k % 13;
        }
        res_count = 13;

        /* sort residues ascending (simple bubble sort) */
        for (size_t a = 0; a < res_count; ++a) {
            for (size_t b = a + 1; b < res_count; ++b) {
                if (residues[b] < residues[a]) {
                    seq_t tmp = residues[a];
                    residues[a] = residues[b];
                    residues[b] = tmp;
                }
            }
        }

        /* remove duplicates, shifting left */
        size_t uniq = 1;
        for (size_t j = 1; j < res_count; ++j) {
            if (residues[j] != residues[uniq - 1]) {
                residues[uniq++] = residues[j];
            }
        }
        res_count = uniq;               /* now res_count == 7 */
        init = true;
    }

    /* fetch the n-th element, cycling modulo the number of residues */
    size_t idx = (size_t)((n - 1) % (idx_t)res_count);
    return residues[idx];
}
