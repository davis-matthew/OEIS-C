/*
 * A010417  Squares mod 56.
 * https://oeis.org/A010417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squares modulo 56, sorted residues repeated (period 12) */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t residues[56];
    static size_t residues_len = 0;

    if (!init) {
        bool seen[56] = { false };
        residues_len = 0;

        /* collect distinct residues of i^2 modulo 56 for i = 0..55 */
        for (idx_t i = 0; i < 56; ++i) {
            uint64_t r = ((uint64_t)i * (uint64_t)i) % 56ULL;
            if (!seen[(size_t)r]) {
                seen[(size_t)r] = true;
                residues[residues_len++] = (seq_t)r;
            }
        }

        /* sort the collected residues in ascending order */
        for (size_t i = 0; i < residues_len; ++i) {
            for (size_t j = i + 1; j < residues_len; ++j) {
                if (residues[j] < residues[i]) {
                    seq_t tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
        init = true;
    }

    return residues[(n - 1) % residues_len];
}
