/*
 * A018886  Waring's problem: least positive integer requiring maximum number
 *          of terms when expressed as a sum of positive n-th powers.
 * https://oeis.org/A018886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^n*floor((3/2)^n) - 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1. */
    if (n == 0) return 0;   /* safe guard – not used for OEIS offset 1 */

    seq_t pow2 = 1;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
    }
    seq_t q = pow3 / pow2;          /* floor((3/2)^n) */
    seq_t result = pow2 * q - 1;    /* 2^n * floor((3/2)^n) - 1 */
    return result;
}
