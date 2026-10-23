/*
 * A192468  Constant term of the reduction by x^2->x+3 of the polynomial
 *          p(n,x)=1+x^n+x^(2n).
 * https://oeis.org/A192468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192468.
   a(n) = 1 + C(n) + C(2n)
   where C(k) is the constant term of the reduction of x^k modulo
   x^2 → x+3.  C(0)=1, C(1)=0 and for k≥2:
   C(k) = 3*A(k-1),
   A(k) = A(k-1)+C(k-1), where A(k) is the coefficient of x in the
   reduced form of x^k. */
/* The following implementation computes C(k) iteratively for all
   required k in O(n) time. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* out of defined range */

    /* pre‑compute constant terms up to 2n */
    idx_t m = n * 2;                   /* 2n, never overflows for valid n */

    /* a_k = coeff of x in x^k,   b_k = constant term in x^k
       Initial values: k=0: a=0, b=1; k=1: a=1, b=0 */
    seq_t a_k = (seq_t)1, b_k = (seq_t)0;
    /* store required constant terms */
    seq_t bn    = (seq_t)0;            /* C(n)   */
    seq_t b2n   = (seq_t)0;            /* C(2n)  */

    if (n == 1) bn = b_k;              /* already set to 0 */

    /* iterate for k = 2 .. m */
    for (idx_t k = 2; k <= m; ++k) {
        seq_t next_a = a_k + b_k;      /* A(k)   */
        seq_t next_b = (seq_t)3 * a_k; /* C(k)   */

        a_k = next_a;
        b_k = next_b;

        if (k == n)  bn = b_k;
        if (k == m)  b2n = b_k;
    }

    return (seq_t)1 + bn + b2n;
}
