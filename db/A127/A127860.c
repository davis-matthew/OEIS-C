/*
 * A127860  a(n) = A127858(n)^2.
 * https://oeis.org/A127860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (5*(12^n-1)/11+1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed, may be omitted */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1, safety for n=0 */

    /* compute 12^n using binary exponentiation */
    seq_t pow12 = 1;
    seq_t base  = 12;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) pow12 = pow12 * base;
        base = base * base;
        e >>= 1;
    }

    seq_t temp = pow12 - 1;           /* 12^n - 1, divisible by 11 */
    seq_t div  = temp / 11;           /* (12^n - 1) / 11  */
    seq_t v    = div * 5 + 1;         /* A127858(n) = 5*(12^n-1)/11 + 1 */
    return v * v;                     /* square to get a(n) */
}
