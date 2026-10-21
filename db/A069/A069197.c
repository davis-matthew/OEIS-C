/*
 * A069197  a(1)=1 a(2)=4 a(n+2)=(a(n+1)+a(n))/3 if (a(n+1)+a(n)==0 (mod 3));
 *          a(n+2)=(a(n+1)+a(n))/2 if (a(n+1)+a(n)==0 (mod 2));
 *          a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4
   a(n+2) = (a(n+1)+a(n))/3 if (a(n+1)+a(n)) mod 3 == 0
          = (a(n+1)+a(n))/2 if (a(n+1)+a(n)) mod 3 != 0 and mod 2 == 0
          = a(n+1)+a(n)     otherwise
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)4;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = prev2 + prev1;
        if ((sum % 3) == 0) {
            next = sum / 3;
        } else if ((sum % 2) == 0) {
            next = sum / 2;
        } else {
            next = sum;
        }
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
