/*
 * A134864  Wythoff BBB numbers.
 * https://oeis.org/A134864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(B(B(n))) where B(k) = ((3*k) + floor(k*sqrt(5))) / 2 *
 *            * This implementation computes the upper Wythoff sequence B(k)
 *            exactly * using only integer arithmetic: * * B(k) = ⌊ k * φ² ⌋ *
 *            = ⌊ k * ( (3+√5)/2 ) ⌋ * = ⌊ (3k + k√5) / 2 ⌋ * = ( 3k + ⌊ k√5 ⌋
 *            ) / 2 * * floor(k√5) is obtained as floo...
 * Range:     u8 none, u16 n <= 8, u32 n <= 1714, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t B_term(idx_t k)
{
    /* k * sqrt(5)  ==  sqrt(5 * k²)  */
    seq_t ksq   = (seq_t)k * (seq_t)k;               /* k²          */
    seq_t sqrt_k5 = floor_sqrt(ksq * 5);            /* ⌊k√5⌋       */
    return ((seq_t)3 * k + sqrt_k5) / 2;             /* B(k)        */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, but harmless */
    seq_t t1 = B_term(n);              /* B(n)     */
    seq_t t2 = B_term((idx_t)t1);      /* B(B(n))  */
    seq_t t3 = B_term((idx_t)t2);      /* B(B(B(n)) */
    return t3;
}
