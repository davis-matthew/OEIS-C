/*
 * A136638  a(n) = Sum_{k=0..[n/2]} C(n-k, k) * C(3^(n-2*k)*2^k, n-k).
 * https://oeis.org/A136638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n‑k, k) * C( 3^(n‑2k) * 2^k , n‑k )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Desired utilities */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute binomial coefficient with a very large top M (as seq_t)
   and a small bottom r (as idx_t).  The result fits in seq_t. */
static seq_t binom_big_top(const seq_t M, idx_t r)
{
    if (r == 0) return 1;          /* C(M,0) = 1  */
    seq_t res = 1;                 /* start with 1 */
    for (idx_t i = 1; i <= r; ++i) {
        /* res = res * (M - (i-1)) / i */
        res = res * (M - (i - 1)); /* multiply by next factor */
        res = res / i;             /* divide by i */
    }
    return res;
}

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
