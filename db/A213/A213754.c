/*
 * A213754  Principal diagonal of the convolution array A213753.
 * https://oeis.org/A213754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*2^n + 3*2^(2*n) - n*2^(n+1) - n^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = 3*2^(2*n) - 3*2^n - n*2^(n+1) - n^2 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0
       (the value is irrelevant because the offset is 1). */
    if (n == 0) {
        return 0;
    }

    /* Compute the required powers of two once. */
    seq_t pow2n    = pow2(n);          /* 2^n          */
    seq_t pow2_2n  = pow2(n + n);      /* 2^(2n)       */
    seq_t pow2_n1  = pow2(n + 1);      /* 2^(n+1)      */

    /* Convert the index to the sequence type for arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* Evaluate the formula using unsigned arithmetic. */
    seq_t result = (seq_t)3 * pow2_2n;   /* 3 * 2^(2n)   */
    result -= (seq_t)3 * pow2n;         /* - 3 * 2^n    */
    result -= n_seq * pow2_n1;          /* - n * 2^(n+1)*/
    result -= n_seq * n_seq;            /* - n^2        */

    return result;
}
