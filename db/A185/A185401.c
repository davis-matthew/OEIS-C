/*
 * A185401  a(n) = (7^n/n!^2) * Product_{k=0..n-1} (14k+2)*(14k+5).
 * https://oeis.org/A185401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7^n / (n!)^2) * Π_{k=0}^{n-1} (14k+2)(14k+5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function assumes that seq_t supports arithmetic
   operators +, -, *, /, % and can be instantiated from
   integer literals.  The type idx_t is an unsigned
   integer. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t k = i - 1;
        seq_t term1 = (seq_t)(14 * k + 2);    /* 14k+2 */
        seq_t term2 = (seq_t)(14 * k + 5);    /* 14k+5 */
        seq_t denom = (seq_t)i * (seq_t)i;    /* i^2 */

        /* Multiply first, then divide.  The division is exact
           by definition of the sequence. */
        res = ((res * (seq_t)7 * term1 * term2) / denom);
    }
    return res;
}
