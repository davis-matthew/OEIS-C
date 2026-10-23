/*
 * A010455  Squares mod 94.
 * https://oeis.org/A010455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the (n-1 mod 48)-th element of the sorted set of
 *            quadratic residues modulo 94 (the sequence repeats every 48
 *            terms)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t residues[48];
    static bool initialized = false;

    if (!initialized) {
        bool seen[94] = {false};
        size_t count = 0;

        /* Collect all distinct residues of k^2 modulo 94. */
        for (idx_t k = 0; k < 94; ++k) {
            idx_t r = (k * k) % 94;
            if (!seen[r]) {
                seen[r] = true;
                residues[count++] = (seq_t)r;
            }
        }

        /* The set should contain exactly 48 residues. */
        (void)count; /* silence unused warning; the count is guaranteed to be 48 */

        /* Sort the residues ascending. */
        for (size_t i = 0; i < count - 1; ++i) {
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

    size_t idx = ((size_t)(n - 1)) % 48;
    return residues[idx];
}
