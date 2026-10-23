/*
 * A028753  Nonsquares mod 40.
 * https://oeis.org/A028753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth positive integer that is not a quadratic residue
 *            modulo 40
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Residues that are not squares modulo 40 (in ascending order). */
    static seq_t residues[31];
    static bool initialized = false;

    if (!initialized) {
        bool is_square[40] = {false};
        for (int a = 0; a < 40; ++a)
            is_square[(a * a) % 40] = true;

        int idx = 0;
        for (int r = 0; r < 40; ++r)
            if (!is_square[r])
                residues[idx++] = (seq_t)r;

        initialized = true;
    }

    idx_t pos = n - 1;        /* zero‑based index */
    idx_t r_index = pos % 31; /* which residue in the list */
    idx_t block = pos / 31;   /* how many full 40‑length blocks have passed */

    return residues[r_index] + (seq_t)block * 40;
}
