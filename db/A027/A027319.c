/*
 * A027319  a(n) = Sum_{k=0..m} (k+1) * A026082(n, k), where 0 <= k <= m, m=n
 *          for n=0,1,2,3; m=2n for n >= 4.
 * https://oeis.org/A027319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=8, a(3)=20, a(n)=8*(n+1)*3^(n-3) for n>=4
 *            Sequence A027319
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)20;

    /* n>=4 */
    idx_t exp = n - 3;               /* exponent for 3 */
    seq_t base = (seq_t)8 * ((seq_t)n + (seq_t)1);  /* 8*(n+1) */
    seq_t pow = pow3(exp);              /* 3^(n-3) */
    return base * pow;
}
