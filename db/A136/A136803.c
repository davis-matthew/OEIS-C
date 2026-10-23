/*
 * A136803  Squares mod 257.
 * https://oeis.org/A136803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn
 * Formula:   a(n) is the n-th smallest quadratic residue modulo 257
 *            (including 0). The residues are obtained by squaring every
 *            integer k with 0 <= k < 257, taking the result modulo 257, and
 *            then sorting the distinct values
 * Range:     u8 n <= 128, u16 n <= 129, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* There are (257+1)/2 = 129 distinct residues. */
    static seq_t res[129];
    static bool built = false;

    if (!built) {
        /* Find all residues k² mod 257. */
        bool seen[257] = {0};
        for (idx_t k = 0; k <= 256; ++k) {
            idx_t r = (k * k) % 257ULL;
            seen[r] = true;
        }
        /* Store the residues in ascending order. */
        idx_t pos = 0;
        for (idx_t v = 0; v < 257; ++v) {
            if (seen[v]) {
                res[pos++] = (seq_t)v;
            }
        }
        built = true;
    }

    if (n >= 1 && n <= 129) {
        return res[n - 1];
    }
    /* Return a sentinel for out-of-range indices. */
    return (seq_t)257;
}
