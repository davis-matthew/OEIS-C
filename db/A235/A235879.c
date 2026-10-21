/*
 * A235879  Number of (n+1) X (3+1) 0..2 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(12 + 7*2^(1+n) + 3^n)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A235879 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) using the provided pow2 routine */
    seq_t pow2n1 = pow2(n + 1);   /* 2^(n+1) */
    /* Compute 3^n using the provided pow3 routine */
    seq_t pow3n  = pow3(n);       /* 3^n */

    seq_t sum = pow3n;                          /* start with 3^n */
    sum = sum + pow2n1 * ((seq_t)7);            /* add 7*2^(n+1) */
    sum = sum + (seq_t)12;                      /* add the constant 12 */
    return sum * ((seq_t)3);                    /* multiply by 3 */
}
