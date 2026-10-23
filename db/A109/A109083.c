/*
 * A109083  Convolution of A002324 and A010815.
 * https://oeis.org/A109083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>=1} (-1)^(k-1)*k * ([n = k*(3k-1)/2] - [n =
 *            k*(3k+1)/2])
 * Range:     i8 n <= 116, i16 n <= 950, i32 n <= 950, i64 n <= 950,
 *            i128 n <= 950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A109083 for all n >= 1. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* sequence is defined for n >= 1, return 0 for safety */
    seq_t result = 0;
    /* iterate over k until both generalized pentagonal numbers exceed n   */
    for (idx_t k = 1; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t n_minus = k_seq * ((seq_t)3 * k_seq - 1) / 2;
        if (n_minus == (seq_t)n) {
            /* coefficient for minus case: (-1)^(k-1) * k */
            if (k & 1)      /* k odd */
                result += k_seq;
            else            /* k even */
                result -= k_seq;
        }
        seq_t n_plus = k_seq * ((seq_t)3 * k_seq + 1) / 2;
        if (n_plus == (seq_t)n) {
            /* coefficient for plus case: (-1)^k * k */
            if ((k & 1) == 0)   /* k even */
                result += k_seq;
            else                /* k odd */
                result -= k_seq;
        }
        /* stop when both are larger than n */
        if (n_minus > (seq_t)n && n_plus > (seq_t)n) break;
    }
    return result;
}
