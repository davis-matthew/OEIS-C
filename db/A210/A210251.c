/*
 * A210251  Residues modulo 100 of odd squares.
 * https://oeis.org/A210251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth smallest residue modulo 100 of odd squares,
 *            computed from the set {1,9} + {0,1,2,3,4}*20 union {25}
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of the 11 sorted residues. */
    static bool initialized = false;
    static seq_t residues[11];

    if (!initialized) {
        /* Build the set {1,9}+{0..4}*20 plus 25. */
        size_t pos = 0;
        const int add = 20;
        for (int i = 0; i < 5; ++i) {
            residues[pos++] = (seq_t)1 + (seq_t)i * (seq_t)add;
            residues[pos++] = (seq_t)9 + (seq_t)i * (seq_t)add;
        }
        residues[pos++] = (seq_t)25;        /* now pos == 11 */

        /* Simple insertion sort (11 elements is negligible). */
        for (size_t i = 1; i < 11; ++i) {
            seq_t key = residues[i];
            size_t j = i;
            while (j > 0 && key < residues[j - 1]) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }

        initialized = true;
    }

    /* n ≥ 1 is guaranteed by the contract. */
    if (n - 1 < 11) {
        return residues[n - 1];
    } else {
        /* For n beyond the finite sequence, return 0 (undefined). */
        return (seq_t)0;
    }
}
