/*
 * A024506  a(n) = [ C(2n,n)/2^(n+3) ].
 * https://oeis.org/A024506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ binomial(2n,n) / 2^(n+3) ⌋
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes a(n) from OEIS A024506: a(n) = floor( C(2n,n)/2^(n+3) ). */
seq_t a(idx_t n)
{
    seq_t B = 1;                       /* B = C(0,0) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Recurrence: C(2i,i) = C(2i-2,i-1) * (4i-2) / i    */
        B = B * (seq_t)(4 * i - 2) / (seq_t)i;
    }
    seq_t denom = pow2(n + 3);          /* 2^(n+3) */
    return B / denom;                  /* integer truncation = floor */
}
