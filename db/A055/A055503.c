/*
 * A055503  Take n points in general position in the plane; draw all the
 *          (infinite) straight lines joining them; sequence gives number of
 *          connected regions formed.
 * https://oeis.org/A055503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1 for n = 0,1; a(2) = 2; for n > 2: a(n) =
 *            (1/8)*(n-1)*(n^3-5*n^2+18*n-8) (OEIS A055503)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 263, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t sn = (seq_t)n;

    /* Compute n^3 - 5*n^2 + 18*n - 8 */
    seq_t n_sq = sn * sn;            /* n^2 */
    seq_t n_cu = n_sq * sn;          /* n^3 */
    seq_t term = n_cu - (seq_t)5 * n_sq + (seq_t)18 * sn - (seq_t)8;

    /* Multiply by (n-1) and divide by 8 */
    seq_t numerator = (sn - (seq_t)1) * term;
    return numerator / (seq_t)8;
}
