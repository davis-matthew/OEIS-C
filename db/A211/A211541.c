/*
 * A211541  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w=3x-4y.
 * https://oeis.org/A211541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of ordered triples (w,x,y) with w,x,y∈{1,…,n} satisfying
 *            2w = 3x – 4y
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts legal (x,y) pairs directly.
   For each even x∈[1,n] we compute the interval of y that
   yields a valid w.  The sum over all even x gives the result. */

#include <stdint.h>

/* Small helper: ceil division of a by b (b>0).  Works for negative a. */
static inline int64_t ceil_div(int64_t a, int64_t b) {
    if (a >= 0)
        return (a + b - 1) / b;
    else
        return a / b;          /* trunc toward zero gives the ceil for negatives */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t x = 2; x <= n; x += 2) {        /* x must be even */
        int64_t num = (int64_t)3 * (int64_t)x; /* 3x                */
        int64_t low_num  = num - (int64_t)2 * (int64_t)n; /* 3x-2n        */
        int64_t high_num = num - 2;                     /* 3x-2         */

        int64_t L = 1;
        int64_t ceilLow = ceil_div(low_num, 4);
        if (ceilLow > L) L = ceilLow;

        int64_t U = (high_num) / 4;          /* floor((3x-2)/4)  */
        if (U > (int64_t)n) U = n;

        if (U >= L) {
            sum += (seq_t)(U - L + 1);
        }
    }
    return sum;
}
