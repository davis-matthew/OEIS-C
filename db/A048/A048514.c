/*
 * A048514  a(n) = T(0,n)+T(1,n-1)+...+T(n,0), array T given by A048505.
 * https://oeis.org/A048514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A048514
   a(n) = T(0,n)+T(1,n-1)+...+T(n,0)
   Formula: a(0) = 1
            a(n) = (n+1)*(n+2)*2^(n-1) - n*(2*n+7)*(n+1)/6   (n >= 1)
*/
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t n_seq   = (seq_t)n;
    seq_t term1   = (n_seq + 1) * (n_seq + 2) * pow2(n - 1);
    seq_t term2   = n_seq * ((seq_t)2 * n_seq + 7) * (n_seq + 1) / 6;
    return term1 - term2;
}
