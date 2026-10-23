/*
 * A027334  a(n) = Sum_{k=0..m} (k+1) * A026148(n, m-k), where m=0 for n=1;
 *          m=n+1 for n >= 2.
 * https://oeis.org/A027334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(n)=2*3^n for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    seq_t p = pow3(n);          /* 3^n */
    return seq_mul(p, (seq_t)2);/* 2*3^n */
}
