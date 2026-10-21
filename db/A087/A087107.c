/*
 * A087107  This table shows the coefficients of combinatorial formulas needed
 *          for generating the sequential sums of p-th powers of tetrahedral
 *          numbers. The p-th row (p>=1) contains a(i,p) for i=1 to 3*p-2,
 *          where a(i,p) satisfies Sum_{i=1..n} C(i+2,3)^p = 4 * C(n+3,4) *
 *          Sum_{i=1..3*p-2} a(i,p) * C(n-1,i-1)/(...
 * https://oeis.org/A087107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = value of the row–major coefficient table for tetrahedral
 *            sums
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 42, u64 n <= 103,
 *            u128 n <= 264, big any n
 * Helpers:   binom_small, seq_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The n-th term is obtained by first locating the corresponding row p and
   column i and then evaluating
   a(i,p) = Σ_{k=1}^{⌈i/2⌉} [ C(i‑1,2k‑2)·C(i‑2k+4,i‑2k+1)^(p‑1)
                             - C(i‑1,2k‑1)·C(i‑2k+3,i‑2k)^(p‑1) ]    */
/* (indices 1‑based, as in the OEIS entry).                                 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom_small.h"   /* binom_small(int n, int k) -> seq_t */
#include "../utils/seq_pow_seq.h"   /* seq_pow_seq(seq_t base, idx_t exp) */

static seq_t binom_or_zero(int n, int k)
{
    if (n < 0 || k < 0 || k > n) return 0;
    return binom_small(n, k);
}

/* Compute a(i,p) where 1 ≤ i ≤ 3p‑2,  p ≥ 1 (both 1‑based). */
static seq_t a_at(idx_t p, idx_t i)
{
    idx_t kmax = (i + 1) / 2;        /* ceil(i/2) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        int a1 = (int)(i - 1);
        int b1 = (int)(2 * (int)k - 2);
        seq_t c1 = binom_or_zero(a1, b1);

        int bb1_n = (int)(i - 2 * (int)k + 4);
        int bb1_k = (int)(i - 2 * (int)k + 1);
        seq_t base1 = binom_or_zero(bb1_n, bb1_k);
        seq_t pow1 = seq_pow_seq(base1, p - 1);   /* (p‑1) ≥ 0 */
        seq_t term1 = c1 * pow1;

        int a2 = a1;                /* same as a1 */
        int b2 = (int)(2 * (int)k - 1);
        seq_t c2 = binom_or_zero(a2, b2);

        int bb2_n = (int)(i - 2 * (int)k + 3);
        int bb2_k = (int)(i - 2 * (int)k);
        seq_t base2 = binom_or_zero(bb2_n, bb2_k);
        seq_t pow2 = seq_pow_seq(base2, p - 1);
        seq_t term2 = c2 * pow2;

        sum += term1;
        sum -= term2;              /* safe: term1 ≥ term2 for every k */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    /* Find row p: smallest p with L(p) = p(3p‑1)/2 ≥ n. */
    idx_t p = 1;
    idx_t L = 1;                   /* L(1) = 1*(3‑1)/2 = 1 */
    while (L < n)
    {
        ++p;
        L = p * (3 * p - 1) / 2;
    }
    /* Number of terms before this row */
    idx_t prevL = (p - 1) * (3 * (p - 1) - 1) / 2;
    idx_t i = n - prevL;           /* 1‑based column within the row */
    return a_at(p, i);
}
