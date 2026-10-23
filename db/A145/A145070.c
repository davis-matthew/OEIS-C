/*
 * A145070  Partial sums of A006127, starting at n=1.
 * https://oeis.org/A145070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-4 + 2^(n+2) + n + n^2) / 2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Compute 2^(n+2) */
    seq_t pow2_nplus2 = pow2(n + 2);

    /* Convert index to seq_t */
    seq_t ns = (seq_t)n;

    /* Compute n^2 */
    seq_t nsq = ns * ns;

    /* Combine according to the closed form */
    seq_t sum = pow2_nplus2 + ns + nsq - (seq_t)4;

    /* Divide by 2 to get the final term */
    return sum / 2;
}
