/*
 * A010383  Squares mod 21.
 * https://oeis.org/A010383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest distinct quadratic residue modulo 21.
 *            That is, a = sorted list of {k^2 mod 21 | k ∈ ℕ}. The list is
 *            [0, 1, 4, 7, 9, 15, 16, 18]. For n > 8 the sequence is not
 *            defined (returns 0)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The residues are independent of n; compute them once. */
    static bool initialized = false;
    static seq_t residues[8];

    if (!initialized) {
        /* Find all distinct squares mod 21. */
        bool seen[21] = {false};
        for (idx_t i = 0; i < 21; ++i) {
            idx_t r = (i * i) % 21;
            seen[r] = true;
        }

        /* Store them in sorted order. */
        idx_t pos = 0;
        for (idx_t r = 0; r < 21; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        /* pos == 8 for modulus 21. */
        initialized = true;
    }

    if (n <= 8) {
        return residues[n - 1];
    }
    /* Undefined for n > 8 – return 0. */
    return (seq_t)0;
}
