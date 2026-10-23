/*
 * A164560  Partial sums of A164532.
 * https://oeis.org/A164560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n=1, 5 if n=2, and for n>2 a(n)=6*a(n-2)+5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1 */

    seq_t odd_val  = 1;           /* a(1) */
    seq_t even_val = 5;           /* a(2) */

    if (n == 1) return odd_val;
    if (n == 2) return even_val;

    for (idx_t i = 3; i <= n; ++i)
    {
        if ((i & 1) == 1)         /* odd index */
            odd_val  = 6 * odd_val  + 5;
        else                      /* even index */
            even_val = 6 * even_val + 5;
    }

    return (n & 1) ? odd_val : even_val;
}
