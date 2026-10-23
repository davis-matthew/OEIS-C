/*
 * A010398  Squares mod 37.
 * https://oeis.org/A010398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th smallest element of { k² mod 37 | k = 0,…,36 }
 *            The set contains 19 distinct values (including 0). The function
 *            returns a(n) for 1 ≤ n ≤ 19; for n > 19 it returns 0
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool built = false;
    static seq_t values[19];
    static idx_t len = 0;

    if (!built) {
        /* Build the list of residues */
        for (idx_t k = 0; k < 37; ++k) {
            seq_t r = (seq_t)k * (seq_t)k % (seq_t)37;
            /* Check if r already present */
            bool present = false;
            for (idx_t i = 0; i < len; ++i) {
                if (values[i] == r) { present = true; break; }
            }
            if (!present) {
                values[len++] = r;
            }
        }
        /* Sort ascending (Insertion sort for such a small fixed size) */
        for (idx_t i = 1; i < len; ++i) {
            seq_t key = values[i];
            idx_t j = i;
            while (j > 0 && values[j - 1] > key) {
                values[j] = values[j - 1];
                --j;
            }
            values[j] = key;
        }
        built = true;
    }

    /* Access by 1‑based index */
    if (n == 0 || n > len) return (seq_t)0;
    return values[n - 1];
}
