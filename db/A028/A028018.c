/*
 * A028018  Expansion of 1/((1-2x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A028018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 336*a(n-2) + 1436*a(n-3) - 1760*a(n-4) with
 *            a(0)=1, a(1)=31, a(2)=625, a(3)=10395
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)625;
    if (n == 3) return (seq_t)10395;

    /* iterate using recurrence */
    seq_t prev4 = (seq_t)1;        /* a(0) */
    seq_t prev3 = (seq_t)31;       /* a(1) */
    seq_t prev2 = (seq_t)625;      /* a(2) */
    seq_t prev1 = (seq_t)10395;    /* a(3) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = ((seq_t)31) * prev1
             - ((seq_t)336) * prev2
             + ((seq_t)1436) * prev3
             - ((seq_t)1760) * prev4;

        /* shift previous terms */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
