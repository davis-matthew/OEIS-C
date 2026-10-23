/*
 * A010390  Squares mod 28.
 * https://oeis.org/A010390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth element (1‑based) of the sorted set of distinct
 *            residues of i² modulo 28 (i = 0…27), repeated periodically
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build the sorted list of unique quadratic residues modulo 28 once. */
    static seq_t residues[8];
    static size_t count = 0;

    if (count == 0) {
        seq_t temp[28];
        size_t m = 0;

        /* Find all distinct residues i² mod 28 for i = 0…27. */
        for (idx_t i = 0; i < 28; ++i) {
            seq_t r = (seq_t)i * (seq_t)i % (seq_t)28;
            bool found = false;
            for (size_t j = 0; j < m; ++j) {
                if (r == temp[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                temp[m++] = r;
            }
        }

        /* Simple insertion sort to order the residues ascending. */
        for (size_t i = 1; i < m; ++i) {
            seq_t key = temp[i];
            size_t j = i;
            while (j > 0 && temp[j - 1] > key) {
                temp[j] = temp[j - 1];
                --j;
            }
            temp[j] = key;
        }

        for (size_t i = 0; i < m; ++i) {
            residues[i] = temp[i];
        }
        count = m; /* Should be 8 */
    }

    if (n == 0) {
        return residues[0];
    }

    /* Periodically repeat the 8 residues for any n >= 1. */
    size_t idx = ((size_t)(n - 1)) % count;
    return residues[idx];
}
