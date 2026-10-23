/*
 * A128866  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3,4,5} such that the intersection of all n entries contains
 *          exactly one element.
 * https://oeis.org/A128866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * (2^n - 1)^4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* Offset is 1, but guard just in case */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= ((seq_t)2);              /* pow2 = 2^n */
    pow2 -= ((seq_t)1);                  /* pow2 = 2^n - 1 */
    seq_t square = pow2 * pow2;          /* (2^n - 1)^2 */
    seq_t fourth = square * square;      /* (2^n - 1)^4 */
    return ((seq_t)5) * fourth;          /* 5 * (2^n - 1)^4 */
}
