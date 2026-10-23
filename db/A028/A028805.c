/*
 * A028805  Nonsquares mod 92.
 * https://oeis.org/A028805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth positive integer that is not a quadratic residue modulo 92 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Non‑residue residues modulo 92, sorted increasingly */
    static const seq_t nonres[68] = {
        2, 3, 5, 6, 7, 10, 11, 14, 15, 17,
        18, 19, 20, 21, 22, 23, 26, 27, 28, 30,
        31, 33, 34, 35, 37, 38, 39, 40, 42, 43,
        44, 45, 46, 47, 50, 51, 53, 54, 55, 56,
        57, 58, 59, 60, 61, 62, 63, 65, 66, 67,
        68, 70, 71, 74, 75, 76, 78, 79, 80, 82,
        83, 84, 86, 87, 88, 89, 90, 91
    };

    idx_t offset = n - 1;               /* zero‑based position */
    idx_t block  = offset / 68;         /* complete address blocks of width 92 */
    idx_t index  = offset % 68;         /* position in the current block */

    /* base = 92 * block  (fits in a larger type before cast) */
    seq_t base = (seq_t)(block * 92);

    return base + nonres[(size_t)index];
}
