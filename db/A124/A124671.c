/*
 * A124671  Row sums of A126277 = binomial transform of (1, 2, 2, 3, 4, 4, 4,
 *          ...)
 * https://oeis.org/A124671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2 + 2^(n+1) - (n^3 + 5*n)/6
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t numerator = n3 + (seq_t)5 * n_seq;  /* n^3 + 5*n */
    seq_t pow2n1 = pow2(n + 1);        /* 2^(n+1) */
    seq_t div6 = numerator / (seq_t)6; /* (n^3 + 5*n)/6 */
    seq_t temp = pow2n1 - div6;        /* 2^(n+1) - (n^3 + 5*n)/6 */
    seq_t res  = temp - (seq_t)2;      /* subtract the final 2 */
    return res;
}
