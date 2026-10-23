/*
 * A136638  a(n) = Sum_{k=0..[n/2]} C(n-k, k) * C(3^(n-2*k)*2^k, n-k).
 * https://oeis.org/A136638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n‑k, k) * C( 3^(n‑2k) * 2^k , n‑k )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binom_big_top, binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Desired utilities */
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binom_big_top.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t r = n - k;                     /* bottom index of second binomial  */
        seq_t c1 = binomial(r, k);           /* C(n‑k, k)  */
        seq_t M = pow3(r - k) * pow2(k);     /* 3^(n‑2k) * 2^k  */
        seq_t c2 = binom_big_top(M, r);      /* C(M, r)  */
        sum = sum + (c1 * c2);               /* accumulate term */
    }
    return sum;
}
