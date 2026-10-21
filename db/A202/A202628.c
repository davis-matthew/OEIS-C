/*
 * A202628  a(n) = (4*n+1)*(2^(4*n+1)+(-1)^n*2^(2*n+1)+1).
 * https://oeis.org/A202628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(2^(4*n+1)+(-1)^n*2^(2*n+1)+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* compute exponents */
    idx_t exp4 = 4 * n + 1;
    idx_t exp2 = 2 * n + 1;

    /* powers of two */
    seq_t p4 = pow2(exp4);
    seq_t p2 = pow2(exp2);

    /* inner term: 2^(4n+1) + (-1)^n * 2^(2n+1) + 1 */
    seq_t inner;
    if ((n & 1ULL) == 0) {      /* n even: +2^(2n+1) */
        inner = p4 + p2 + 1;
    } else {                    /* n odd : -2^(2n+1) */
        inner = p4 - p2 + 1;
    }

    /* outer factor: (4n+1) */
    seq_t factor = (seq_t)(4 * n + 1);

    /* final result */
    return factor * inner;
}
