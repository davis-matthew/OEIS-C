/*
 * A156296  a(1)=1, a(n) = 2 * Sum_{k=1..n-1} (3^k-1)/2 * a(k) for n>=2.
 * https://oeis.org/A156296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=a(n-1)*(3^n+1)/2 for n>=1
   This follows from the given definition and matches the OEIS values. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;               /* a(0) */
    seq_t pow3   = 1;               /* 3^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow3 *= (seq_t)3;           /* pow3 = 3^i */
        seq_t factor = pow3 + (seq_t)1;
        factor /= (seq_t)2;         /* (3^i + 1)/2 is guaranteed integer */
        result *= factor;          /* a(i) = a(i-1) * factor */
    }

    return result;
}
