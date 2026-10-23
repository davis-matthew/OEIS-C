/*
 * A010451  Squares mod 90.
 * https://oeis.org/A010451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest quadratic residue modulo 90 (i.e. sorted
 *            unique values of i² mod 90 for i ≥ 0)
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t residues[24];
    static size_t count = 0;

    if (!initialized) {
        bool seen[90] = {false};
        size_t idx = 0;

        for (idx_t i = 0; i < 90; ++i) {
            /* Compute (i * i) % 90 using seq_t arithmetic. */
            seq_t sq = ((seq_t)i * (seq_t)i) % 90;
            if (!seen[(idx_t)sq]) {
                seen[(idx_t)sq] = true;
                residues[idx++] = sq;
            }
        }
        count = idx;

        /* Simple insertion sort (count is tiny). */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (residues[j] < residues[i]) {
                    seq_t tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
        initialized = true;
    }

    if (n == 0 || n > count)
        return (seq_t)0;
    return residues[n - 1];
}
