/*
 * A010452  Squares mod 91.
 * https://oeis.org/A010452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Squares mod 91 – sorted distinct quadratic residues modulo 91 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are 28 distinct residues of n^2 modulo 91, 
       given by the CRT combinations of residues 
       modulo 7 and modulo 13. */
    static seq_t res[28];
    static bool initialized = false;

    if (!initialized) {
        /* Quadratic residues modulo 7 and modulo 13. */
        const int R7[4]  = {0, 1, 2, 4};
        const int R13[7] = {0, 1, 3, 4, 9, 10, 12};

        /* Compute all residues via CRT: x = 78*r7 + 14*r13 (mod 91). */
        int k = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 7; ++j) {
                seq_t val = ((seq_t)78 * R7[i]) + ((seq_t)14 * R13[j]);
                val %= (seq_t)91;
                res[k++] = val;
            }
        }

        /* Sort the 28 residues ascending. */
        for (int i = 0; i < 27; ++i) {
            for (int j = i + 1; j < 28; ++j) {
                if (res[j] < res[i]) {
                    seq_t tmp = res[i];
                    res[i] = res[j];
                    res[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    /* Return the residue corresponding to the 1‑based index. */
    idx_t pos = (n - 1) % (idx_t)28;
    return res[pos];
}
