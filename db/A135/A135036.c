/*
 * A135036  Sums of the products of n consecutive pairs of numbers.
 * https://oeis.org/A135036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*n*(4*n + 1)/3 (OEIS A135036)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) is undefined but return 0 */
        return (seq_t)0;

    seq_t nn   = (seq_t)n;           /* n */
    seq_t n1   = (seq_t)(n - 1);     /* n-1 */
    /* compute (n-1)*n first, then multiply by (4*n+1) */
    seq_t prod = n1 * nn;            /* (n-1)*n */
    seq_t mul  = ((seq_t)4 * nn) + (seq_t)1; /* 4*n + 1 */
    prod = prod * mul;              /* (n-1)*n*(4*n+1) */
    return prod / (seq_t)3;         /* divide by 3 (exact integer division) */
}
