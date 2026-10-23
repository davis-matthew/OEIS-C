/*
 * A010423  Squares mod 62.
 * https://oeis.org/A010423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest quadratic residue modulo 62, sorted
 *            ascending
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t residues[62];
    static size_t count = 0;
    static unsigned char visited[62];
    if (count == 0) {
        /* collect all residues */
        for (idx_t i = 0; i < 62; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i % 62;
            visited[(int)sq] = 1;
        }
        /* build sorted list */
        for (int val = 0; val < 62; ++val) {
            if (visited[val]) {
                residues[count++] = (seq_t)val;
            }
        }
    }
    if (n == 0) return 0;
    if ((size_t)(n - 1) < count) return residues[n - 1];
    return 0; /* out of bounds */
}
