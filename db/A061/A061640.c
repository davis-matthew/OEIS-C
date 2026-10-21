/*
 * A061640  a(n) = !n*n!.
 * https://oeis.org/A061640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = n! * Σ_{k=0}^{n-1} k! (with a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 0 */

    seq_t sum = 1;                    /* Σ_{k=0}^{-1} k! = 0! = 1 */
    seq_t fact = 1;                   /* factorial of the current i */

    for (idx_t i = 1; i < n; ++i) {
        fact *= (seq_t)i;             /* fact = i! (for next addition) */
        sum += fact;                  /* add i! to the cumulative sum */
    }

    fact *= (seq_t)n;                  /* fact = n!  */
    return fact * sum;                 /* a(n) = n! * Σ_{k=0}^{n-1} k! */
}
