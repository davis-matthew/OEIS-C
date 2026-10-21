/*
 * A066368  a(n) = (n+2)*2^(n-1) - 2*n.
 * https://oeis.org/A066368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^(n-1) - 2*n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"


seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nplus2 = (seq_t)n + (seq_t)2;   /* n+2 as seq_t */
    seq_t pow    = pow2(n - 1);           /* 2^(n-1) */
    seq_t sub    = ((seq_t)n) << 1;       /* 2*n */
    seq_t prod   = nplus2 * pow;          /* (n+2)*2^(n-1) */
    return prod - sub;                   /* final result */
}
