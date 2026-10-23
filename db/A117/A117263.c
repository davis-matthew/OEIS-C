/*
 * A117263  Row sums of triangle A117262; also, self-convolution of A117264.
 * https://oeis.org/A117263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117263: Row sums of triangle A117262
   Formula: a(n) = Σ_{k=0..n} 3^( n*(n-1)/2 - k*(k-1)/2 )
   Recurrence: a(0)=1 ; a(n)=a(n-1)*3^(n-1)+1  for n≥1
   (derived from the functional equation A(x)=1/(1-x)+xA(3x) )
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;          // a(0)=1

    seq_t value = (seq_t)1;                // current a(k-1), start at a(0)
    seq_t pow3   = (seq_t)1;                // 3^(k-1), start at 3^0

    for (idx_t k = 1; k <= n; ++k)
    {
        // a(k) = value * pow3 + 1
        value = value * pow3 + (seq_t)1;

        // prepare pow3 for next iteration: 3^k
        pow3 *= (seq_t)3;
    }

    return value;
}
