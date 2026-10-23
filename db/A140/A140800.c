/*
 * A140800  Total number of vertices in all finite n-dimensional convex
 *          regular polytopes, or 0 if the number is infinite.
 * https://oeis.org/A140800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=0, a(3)=50, a(4)=773, a(n)=2^n+3*n+1 for
 *            n>=5 (Total number of vertices in all finite n‑dimensional
 *            convex regular polytopes, or 0 if the number is infinite.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)50;
    if (n == 4) return (seq_t)773;
    /* n >= 5 */
    seq_t val = pow2(n);          /* 2^n */
    val += (seq_t)(3 * (seq_t)n); /* 3*n */
    val += (seq_t)1;              /* +1 */
    return val;
}
