/*
 * A028056  Expansion of 1/((1-3x)(1-5x)(1-6x)(1-10x)).
 * https://oeis.org/A028056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=24, a(2)=373, a(3)=4800
   a(n) = 24*a(n-1) - 203*a(n-2) + 720*a(n-3) - 900*a(n-4)   (n ≥ 4) */
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)24;
    if (n == (idx_t)2) return (seq_t)373;
    if (n == (idx_t)3) return (seq_t)4800;

    seq_t a0 = (seq_t)1;     /* a(n-4)   */
    seq_t a1 = (seq_t)24;    /* a(n-3)   */
    seq_t a2 = (seq_t)373;    /* a(n-2)   */
    seq_t a3 = (seq_t)4800;   /* a(n-1)   */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        seq_t hi  = mul_small(a3, 24) + mul_small(a1, 720);
        seq_t lo  = mul_small(a2, 203) + mul_small(a0, 900);
        seq_t cur = hi - lo;          /* hi ≥ lo for all n */

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    return a3;
}
