/*
 * A010407  Squares mod 46.
 * https://oeis.org/A010407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 46*floor((n-1)/24) + r[(n-1) mod 24], where r[0..23] are
 *            the quadratic residues modulo 46 (i.e. the set of numbers x with
 *            ∃k such that k² ≡ x (mod 46)), sorted in increasing order
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t residues[24];
    static bool initialized = false;

    if (!initialized) {
        bool seen[46] = {0};
        idx_t count = 0;

        for (idx_t k = 0; k < 46; ++k) {
            idx_t r = (k * k) % 46;
            if (!seen[r]) {
                seen[r] = true;
                residues[count++] = (seq_t)r;
            }
        }

        /* Sort the residues increasingly */
        for (size_t i = 1; i < (size_t)count; ++i) {
            seq_t key = residues[i];
            size_t j = i;
            while (j > 0 && residues[j - 1] > key) {
                residues[j] = residues[j - 1];
                --j;
            }
            residues[j] = key;
        }

        initialized = true;
    }

    idx_t m = n - 1;
    idx_t q = m / 24;
    idx_t r = m % 24;

    return residues[r] + ((seq_t)q * (seq_t)46);
}
