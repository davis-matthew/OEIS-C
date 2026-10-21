/*
 * A054449  Row sums of triangle A054448 (second member of partial row sums
 *          triangle family of Fibonacci convolution triangle).
 * https://oeis.org/A054449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n^2+17*n+32)*P(n+1)+13*(n+1)*P(n))/32, where P(k) is
 *            the k‑th Pell number (P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* compute Pell numbers P(n) and P(n+1) */
    seq_t Pn  = 0;      /* P(0) */
    seq_t Pnp1 = 1;     /* P(1) */
    for (idx_t i = 2; i <= n+1; ++i) {
        seq_t tmp = Pnp1*2 + Pn;  /* P(i) = 2*P(i-1)+P(i-2) */
        Pn    = Pnp1;
        Pnp1 = tmp;
    }

    seq_t nn  = (seq_t)n;   /* cast index to seq_t for arithmetic */
    seq_t coeff = (seq_t)2*nn*nn + (seq_t)17*nn + (seq_t)32;
    seq_t term1 = coeff * Pnp1;                     /* (2*n^2+17*n+32)*P(n+1) */
    seq_t term2 = (seq_t)13 * (nn + 1) * Pn;        /* 13*(n+1)*P(n) */
    seq_t sum   = term1 + term2;
    return sum / 32;   /* division is exact */
}
