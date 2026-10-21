/*
 * A098938  Duplicate of A098923.
 * https://oeis.org/A098938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(31*n-29)/2
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to the sequence type */
    seq_t nseq = (seq_t)n;
    /* Compute the expression n * (31*n - 29) / 2 */
    seq_t t = nseq * ((seq_t)31 * nseq - (seq_t)29);
    return t / 2;
}
