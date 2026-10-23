/*
 * A163417  a(n) = 2^(floor((n-1)/2)) - n*(n-1)/2.
 * https://oeis.org/A163417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^floor((n-1)/2) - n*(n-1)/2
 * Range:     i8 n <= 18, i16 n <= 32, i32 n <= 64, i64 n <= 128,
 *            i128 n <= 256, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Guard against n == 0, though offset is 1 */
    if (n == 0) return 0;
    idx_t k = (n - 1) / 2;                       /* floor((n-1)/2) */
    seq_t pow = pow2(k);                         /* 2^k */
    seq_t tri = ((seq_t)n * (seq_t)(n - 1)) / 2; /* n*(n-1)/2 */
    return pow - tri;
}
