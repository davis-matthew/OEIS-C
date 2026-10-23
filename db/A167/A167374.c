/*
 * A167374  Triangle, read by rows, given by [ -1,1,0,0,0,0,0,0,0,...] DELTA
 *          [1,0,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A167374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = 1 if n - r*(r+1)/2 = r, -1 if n - r*(r+1)/2 = r-1, 0
 *            otherwise, where r = ⌊(√(8*n + 1) – 1)/2⌋
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   intr_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/intr_isqrt_u64.h"

seq_t a(idx_t n) {
    /* Compute r = floor((sqrt(8*n + 1) - 1)/2) using integer sqrt. */
    idx_t r = (intr_isqrt_u64(8 * n + 1) - 1) >> 1;

    /* Position of n inside its row: j = n - r*(r+1)/2. */
    idx_t j = n - r * (r + 1) / 2;

    if (j == r) {
        return (seq_t)1;
    } else if (j == r - 1) {
        return (seq_t)(-1);
    } else {
        return (seq_t)0;
    }
}
