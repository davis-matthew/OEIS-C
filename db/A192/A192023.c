/*
 * A192023  The Wiener index of the comb-shaped graph |_|_|...|_| with 2n
 *          (n>=1) nodes. The Wiener index of a connected graph is the sum of
 *          the distances between all unordered pairs of vertices in the
 *          graph.
 * https://oeis.org/A192023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 6*n - 5)/3
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1859, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Ensure n is at least 1 as per sequence offset */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;
    /* Compute raw = 2*n^2 + 6*n - 5 */
    seq_t raw = (seq_t)2 * sn * sn + (seq_t)6 * sn - (seq_t)5;

    /* Reduce risk of overflow by dividing by 3 if possible. */
    if (sn % (seq_t)3 == 0) {
        return (sn / (seq_t)3) * raw;
    } else if (raw % (seq_t)3 == 0) {
        return sn * (raw / (seq_t)3);
    } else {
        return (sn * raw) / (seq_t)3;
    }
}
