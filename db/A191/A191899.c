/*
 * A191899  Number of compositions of odd natural numbers into 8 parts <=n.
 * https://oeis.org/A191899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^8 - ((n % 2 == 0) ? 1 : 0)) / 2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute (n + 1) as seq_t */
    seq_t np1 = (seq_t)n + (seq_t)1;

    /* compute (n + 1)^8 by repeated multiplication */
    seq_t p = np1 * np1;   /* (n+1)^2 */
    p = p * p;             /* (n+1)^4 */
    p = p * p;             /* (n+1)^8 */

    /* Q = 1 if n is even, 0 otherwise */
    seq_t q = ((n & (idx_t)1) == 0) ? (seq_t)1 : (seq_t)0;

    /* final result */
    return (p - q) / (seq_t)2;
}
