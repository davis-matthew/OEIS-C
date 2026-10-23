/*
 * A008649  Molien series of 3 X 3 upper triangular matrices over GF( 3 ).
 * https://oeis.org/A008649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/9)+1)*(floor(n/3)+1) -
 *            3*floor(n/9)*(floor(n/9)+1)/2
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 1, 3 or 9. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor divisions */
    idx_t f = n / 3;       /* floor(n/3) */
    idx_t c = n / 9;       /* floor(n/9) */

    /* All arithmetic is done in seq_t to keep precision for arbitrary-size builds.
       Convert the idx_t values to seq_t before multiplication to avoid unintended
       overflow of the 64-bit index type in platforms where idx_t is uint64_t. */
    seq_t term1 = (seq_t)(c + 1) * (seq_t)(f + 1);               /* (c+1)*(f+1)   */
    seq_t prod   = (seq_t)c * (seq_t)(c + 1);                     /* c*(c+1)       */
    prod = prod / 2;                                             /* divide by 2    */
    seq_t term2 = (seq_t)3 * prod;                               /* 3 * c*(c+1)/2 */

    return term1 - term2;
}
