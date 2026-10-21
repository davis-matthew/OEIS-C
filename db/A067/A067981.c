/*
 * A067981  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+3),
 *          n>=0.
 * https://oeis.org/A067981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+4)*F(n+1) + 4*(n+1)*F(n) (F(n) = Fibonacci, F(0)=0,
 *            F(1)=1)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute F(n) and F(n+1) iteratively */
    seq_t Fn = 0;          /* F(0) */
    seq_t Fn1 = 1;         /* F(1) */
    idx_t i;

    for (i = 0; i < n; ++i) {
        seq_t next = Fn + Fn1;   /* F(i+2) */
        Fn = Fn1;
        Fn1 = next;
    }
    /* After loop: Fn = F(n), Fn1 = F(n+1) */

    seq_t nseq = (seq_t)n;
    seq_t term1 = ((seq_t)7 * nseq + (seq_t)4) * Fn1;
    seq_t term2 = ((seq_t)4 * (nseq + (seq_t)1)) * Fn;
    return term1 + term2;
}
