/*
 * A224541  Number of doubly-surjective functions f:[n]->[3].
 * https://oeis.org/A224541
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*2^n - 3*n*2^(n-1) + 3 + 3*n + 3*n^2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n < 6u) return 0;  /* offset: a(6) is first defined term */
    seq_t pow3n = pow3(n);
    seq_t pow2n = pow2(n);
    seq_t pow2nm1 = pow2(n - 1);
    seq_t term = pow3n
                - (seq_t)3 * pow2n
                - (seq_t)3 * (seq_t)n * pow2nm1
                + (seq_t)3
                + (seq_t)3 * (seq_t)n
                + (seq_t)3 * (seq_t)n * (seq_t)n;
    return term;
}
