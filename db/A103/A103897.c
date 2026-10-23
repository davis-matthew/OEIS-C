/*
 * A103897  a(n) = 3*2^(n-1)*(2^n-1).
 * https://oeis.org/A103897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1)*(2^n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, behaviour below offset does not matter */
    seq_t two_n_minus_1 = pow2(n - 1);  /* 2^(n-1) */
    seq_t two_n = pow2(n);              /* 2^n */
    seq_t part = two_n - 1;             /* (2^n - 1) */
    seq_t result = 3 * two_n_minus_1;   /* 3*2^(n-1) */
    result *= part;                     /* * (2^n - 1) */
    return result;
}
