/*
 * A184327  a(1)=1, a(2)=17; thereafter a(n) = 6*a(n-1)-a(n-2)+c, where c=-4
 *          if n is odd, c=12 if n is even.
 * https://oeis.org/A184327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A184327
   a(1) = 1, a(2) = 17;
   a(n) = 6*a(n-1) - a(n-2) + c,
   c = -4 if n is odd, c = 12 if n is even, n ≥ 3.
   Offset: 1
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)17;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)17;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)6 * prev1 - prev2;
        if (i & 1U) {          /* odd n → c = -4 */
            cur -= (seq_t)4;
        } else {               /* even n → c = 12 */
            cur += (seq_t)12;
        }
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
