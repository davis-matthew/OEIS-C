/*
 * A010404  Squares mod 43.
 * https://oeis.org/A010404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th smallest integer y such that y ≡ x² (mod 43) for
 *            some integer x
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t residues[22];
    static bool ready = false;
    if (!ready) {
        bool seen[43] = {0};
        for (idx_t k = 0; k < 43; ++k) {
            seq_t val = (seq_t)k * k % 43;
            seen[(unsigned)val] = true;
        }
        size_t count = 0;
        for (unsigned i = 0; i < 43; ++i) {
            if (seen[i]) {
                residues[count++] = (seq_t)i;
            }
        }
        /* count will be 22 for prime modulus 43 */
        ready = true;
    }
    if (n == 0) return 0;
    if (n <= 22) return residues[n - 1];
    return 0;
}
