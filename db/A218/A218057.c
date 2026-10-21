/*
 * A218057  a(n) = (b(n)+1)^b(n+1) + b(n+2)^(b(n+1)+1)/2, where b(n) = [n/2]*2
 *          = A052928(n) =(0,0,2,2,4,4,...), even numbers repeated.
 * https://oeis.org/A218057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n)+1)^b(n+1) + b(n+2)^(b(n+1)+1)/2 where b(n) =
 *            A052928(n) = floor(n/2)*2 (even numbers repeated)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Even parts of n, n+1, n+2 */
    idx_t bn   = n & (~(idx_t)1);        /* b(n)   */
    idx_t bn1  = (n + 1) & (~(idx_t)1);  /* b(n+1) */
    idx_t bn2  = (n + 2) & (~(idx_t)1);  /* b(n+2) */

    /* (b(n)+1)^(b(n+1))                              */
    seq_t term1 = big_pow((seq_t)(bn + 1), (seq_t)bn1);

    /* b(n+2)^(b(n+1)+1) / 2                          */
    seq_t term2 = big_pow((seq_t)bn2, (seq_t)(bn1 + 1));
    term2 = term2 / (seq_t)2;

    return term1 + term2;
}
