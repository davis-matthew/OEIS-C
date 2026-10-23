/*
 * A157064  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 30.
 * https://oeis.org/A157064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..min(n,15)}
 *            binom(n+1,i)*binom(14,i-1)*binom(n-i+15,15)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 25, u128 n <= 122,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the OEIS sequence A157064: Number of integer sequences of
   length n+1 with sum zero and sum of absolute values 30. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, but guard against it */

    seq_t result = 0;
    /* i only needs to run up to min(n,15) because binom(14,i-1)=0 for i>15 */
    idx_t limit = n < 15 ? n : 15;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t t1 = binom(n + 1, i);          /* C(n+1,i)   */
        seq_t t2 = binom(14, i - 1);         /* C(14,i-1)  */
        seq_t t3 = binom(n - i + 15, 15);    /* C(n-i+15,15)*/
        result += t1 * t2 * t3;
    }
    return result;
}
