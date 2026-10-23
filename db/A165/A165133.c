/*
 * A165133  a(n) = (2^(n+4)-1)*(2^n+1).
 * https://oeis.org/A165133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+4)-1)*(2^n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility power function pow2(idx_t) from "../utils/pow2.h" */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+4) and 2^n using the provided pow2 helper. */
    seq_t pow_n     = pow2(n);        /* 2^n      */
    seq_t pow_n4    = pow2(n + 4);    /* 2^(n+4)  */

    /* (2^(n+4)-1) */
    pow_n4 -= (seq_t)1;

    /* (2^n+1) */
    pow_n += (seq_t)1;

    /* Multiply the two factors. */
    return pow_n4 * pow_n;
}
