/*
 * A027775  a(n) = (n+1)*binomial(n+1, 15).
 * https://oeis.org/A027775
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * binomial(n+1, 15) (offset 14)
 * Range:     u8 none, u16 n <= 17, u32 n <= 28, u64 n <= 97, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We only compute for n >= 14, the behaviour for smaller n is irrelevant. */
    seq_t n1 = (seq_t)n + 1;          /* n+1  */
    seq_t binom = 1;                  /* C(n+1,15) computed multiplicatively */

    for (idx_t i = 1; i <= 15; ++i)   /* i runs from 1 to 15 */
    {
        /* Use the standard recurrence C(n,k) = C(n,k-1) * (n-k+1) / k */
        seq_t num = n1 - (i - 1);     /* numerator: n+1 - (i-1) = n+2-i */
        binom = (binom * num) / (seq_t)i;
    }

    return n1 * binom;                /* (n+1) * C(n+1,15) */
}
