/*
 * A010392  Squares mod 31.
 * https://oeis.org/A010392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = sorted_quadratic_residues_mod31[(n-1) % 16]
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t residues[16];

    if (!initialized) {
        seq_t temp[16];
        size_t cnt = 0;

        /* Enumerate all quadratic residues modulo 31 */
        for (idx_t k = 0; k <= 30; ++k) {
            idx_t prod = k * k;                /* use idx_t to avoid overflow */
            seq_t r = (seq_t)(prod % 31);

            /* check for duplicates */
            bool exists = false;
            for (size_t i = 0; i < cnt; ++i) {
                if (temp[i] == r) { exists = true; break; }
            }
            if (!exists) {
                temp[cnt++] = r;
            }
        }

        /* sort the residues ascending (cnt should be 16) */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = temp[i];
            int j = (int)i - 1;
            while (j >= 0 && temp[j] > key) {
                temp[j + 1] = temp[j];
                --j;
            }
            temp[j + 1] = key;
        }

        /* transfer sorted values to the static array */
        for (size_t i = 0; i < cnt; ++i) {
            residues[i] = temp[i];
        }

        initialized = true;
    }

    idx_t idx = (n - 1) % 16;   /* periodic with period 16 */
    return residues[idx];
}
