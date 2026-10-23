/*
 * A010410  Squares mod 49.
 * https://oeis.org/A010410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010410: Squares modulo 49
   a(n) is the n-th smallest distinct quadratic residue modulo 49.
   The sequence consists of all residues r with 0 <= r < 49 that can be
   written as k^2 mod 49 for some integer k, listed in increasing order. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for residues and count */
    static seq_t residues[49];
    static size_t cnt = 0;
    static bool computed = false;

    if (!computed) {
        bool seen[49] = {false};
        /* compute residues of k^2 mod 49 for k = 0..48 */
        for (idx_t k = 0; k < 49; ++k) {
            seq_t r = ((seq_t)k * (seq_t)k) % (seq_t)49;
            seen[(size_t)r] = true;
        }
        /* collect distinct residues in ascending order */
        size_t pos = 0;
        for (size_t r = 0; r < 49; ++r) {
            if (seen[r]) {
                residues[pos++] = (seq_t)r;
            }
        }
        cnt = pos;
        computed = true;
    }

    if (n == 0 || n > cnt) {
        return (seq_t)0;
    }
    return residues[(size_t)(n - 1)];
}
