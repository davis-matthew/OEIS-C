/*
 * A207636  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207635; see Formula section.
 * https://oeis.org/A207636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n-7)-1)/2), k = n -
 *            r*(r+1)/2 - 1. For r >= 1: T(r,k) = 3*C(r-1,k)*2^{r-1-k} +
 *            2*C(r-1,k-1)*2^{r-k}. For r = 0: T(0,0) = 1
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies: powers of two and binomial coefficients.  */
#include <math.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Determine the row r such that the flattened index n lies in row r. */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 as safe default */

    /* r satisfies: r*(r+1)/2 < n <= (r+1)*(r+2)/2   */
    uint64_t t = 8ULL * (uint64_t)n - 7ULL;          /* 8*n-7 fits in 64 bits for n up to 10000 */
    uint64_t s = (uint64_t)floor(sqrt((long double)t));
    uint64_t r = (s - 1U) / 2U;                      /* floor((sqrt(8*n-7)-1)/2) */

    uint64_t prev = r * (r + 1ULL) / 2ULL;           /* total terms before row r */
    uint64_t k = (uint64_t)n - prev - 1ULL;          /* position within the row (0‑based) */

    if (r == 0U)
        return (seq_t)1;         /* T(0,0) */

    seq_t t1 = 0, t2 = 0;

    if (k <= r - 1U) {
        seq_t comb1 = binom(r - 1U, k);
        seq_t pow1  = pow2((idx_t)((r - 1U) - k));
        t1 = comb1 * pow1;
        t1 = t1 * (seq_t)3;       /* multiply by 3 */
    }

    if (k > 0U) {
        seq_t comb2 = binom(r - 1U, k - 1U);
        seq_t pow2v = pow2((idx_t)(r - k));
        t2 = comb2 * pow2v;
        t2 = t2 * (seq_t)2;       /* multiply by 2 */
    }

    return t1 + t2;
}
