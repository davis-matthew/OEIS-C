/*
 * A010445  Squares mod 84.
 * https://oeis.org/A010445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth smallest quadratic residue modulo 84 (sorted in
 *            ascending order)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute the sorted list of distinct squares modulo 84 once. */
    static seq_t residues[16];
    static size_t residues_size = 0;

    if (residues_size == 0) {
        bool seen[84] = { false };
        size_t count = 0;
        for (idx_t x = 0; x < 84; ++x) {
            seq_t r = ((seq_t)x * (seq_t)x) % (seq_t)84;
            /* Ensure r fits in the array index (it is < 84). */
            if (!seen[(int)r]) {
                seen[(int)r] = true;
                residues[count++] = r;
            }
        }
        /* Sort the residues in ascending order (simple insertion sort). */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (residues[j] < residues[i]) {
                    seq_t tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
        residues_size = count;  /* Should be 16. */
    }

    /* False overflow: n >= 1, so n-1 fits in idx_t. */
    idx_t idx0 = n - 1;
    /* Periodic repetition if n exceeds the number of distinct residues. */
    size_t index = (size_t)(idx0 % (idx_t)residues_size);
    return residues[index];
}
