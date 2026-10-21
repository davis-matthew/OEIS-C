/*
 * A212851  Number of n X 4 arrays with rows being permutations of 0..3 and no
 *          column j greater than column j-1 in all rows.
 * https://oeis.org/A212851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24^n + 6^n - 3*12^n + 2*4^n - 1 (OEIS A212851)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   pow_small_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_small_uint.h"

seq_t a(idx_t n) {
    /* n is 1-based index; compute formula directly */
    seq_t p24 = pow_small_uint((seq_t)24, n);
    seq_t p6  = pow_small_uint((seq_t)6, n);
    seq_t p12 = pow_small_uint((seq_t)12, n);
    seq_t p4  = pow_small_uint((seq_t)4, n);

    seq_t result = p24 + p6 - (seq_t)3 * p12 + (seq_t)2 * p4 - (seq_t)1;
    return result;
}
