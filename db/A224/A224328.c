/*
 * A224328  Number of idempotent n X n 0..3 matrices of rank n-1.
 * https://oeis.org/A224328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*4^(n-1)-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* For n >= 1 */
    if (n == 0) return 0;                   /* undefined for n<1, return 0 safely */
    seq_t pow_term = pow_seq((seq_t)4, (int)(n - 1));   /* 4^(n-1) */
    seq_t temp     = (seq_t)2 * pow_term - (seq_t)1;    /* 2*4^(n-1) - 1 */
    return (seq_t)n * temp;                                 /* n * (2*4^(n-1)-1) */
}
