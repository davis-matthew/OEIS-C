/*
 * A160873  Number of isomorphism classes of connected (D_4)-fold coverings of
 *          a connected graph with circuit rank n.
 * https://oeis.org/A160873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * (2^n - 1) * (2^(n-1) - 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    seq_t p_n  = pow2(n);     /* 2^n   */
    seq_t p_nm1 = pow2(n-1);  /* 2^(n-1) */
    seq_t p_nm2 = pow2(n-2);  /* 2^(n-2) # n>=2 so n-2>=0 */
    
    /* (2^n - 1) * (2^(n-1) - 1) */
    seq_t term = (p_n - (seq_t)1) * (p_nm1 - (seq_t)1);
    
    return p_nm2 * term;
}
