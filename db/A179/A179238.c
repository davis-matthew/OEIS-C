/*
 * A179238  Numerators in convergents to infinitely repeating period 3
 *          palindromic continued fraction [1,2,1,...].
 * https://oeis.org/A179238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A179238: Numerators in convergents to the repeating
   palindromic continued fraction [1,2,1,...].
   Recurrence (indexed from 1):
   a(1)=1, a(2)=2, a(3)=3
   For n>=4:
     if n % 3 == 2   -> a(n) = 2*a(n-1) + a(n-2)
     else            -> a(n) =   a(n-1) + a(n-2)
   Equivalent to a(n) = a(n-1) + a(n-2) for n ≡ 0 or 1 (mod 3),
   and a(n) = 2*a(n-1) + a(n-2) for n ≡ 2 (mod 3).
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 3) {
        /* n is 1,2, or 3 */
        static const seq_t base[4] = {0, 1, 2, 3};
        return base[n];
    }

    /* a_prev1 = a(i-2), a_curr = a(i-1) */
    seq_t a_prev1 = (seq_t)2;   /* a(2) */
    seq_t a_curr  = (seq_t)3;   /* a(3) */

    /* Iterate from i = 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next;
        if ((i % 3) == 2)     /* n ≡ 2 (mod 3) – multiplication rule */
            next = a_curr * (seq_t)2 + a_prev1;
        else                  /* n ≡ 0 or 1 (mod 3) – addition rule */
            next = a_curr + a_prev1;

        /* Shift window for next iteration */
        a_prev1 = a_curr;
        a_curr  = next;
    }

    return a_curr;   /* a(n) */
}
