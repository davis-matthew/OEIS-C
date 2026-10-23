/*
 * A136597  Column 3 of triangle A136595.
 * https://oeis.org/A136597
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=0..n-1}
 *            (-1)^i*(3+i)!*Stirling2(n,3+i)*Catalan(3,i)/3!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorial.h"   /* factorial(idx_t) -> seq_t */
#include "../utils/binomial.h"   /* binomial(idx_t,idx_t) -> seq_t */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t,seq_t) -> seq_t */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    const size_t len = (size_t)n + 3;          /* indices 0 .. n+2 */
    seq_t *prev = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!prev || !curr) { /* allocation failure */
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* initialize Stirling row 0 : S(0,0)=1, others 0 */
    for (size_t i = 0; i < len; ++i) prev[i] = (seq_t)0;
    prev[0] = (seq_t)1;

    /* compute Stirling numbers up to row n */
    for (idx_t row = 1; row <= n; ++row) {
        /* zero curr */
        for (size_t k = 0; k < len; ++k) curr[k] = (seq_t)0;

        /* k ranges 0 .. row+2  (max needed k = 3+i with i=n-1 = n+2) */
        for (idx_t k = 0; k <= row + 2 && k < len; ++k) {
            seq_t val = (seq_t)0;
            if (k <= row + 1)                /* k * S(row-1,k) */
                val = seq_mul(prev[k], (seq_t)k);
            if (k >= 1)                       /* + S(row-1,k-1) */
                val = val + prev[k - 1];
            curr[k] = val;
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* pre‑compute factorials and Catalan(3,i) for i = 0 .. n-1 */
    seq_t *fac = (seq_t *)malloc((size_t)n * sizeof(seq_t)); /* (i+3)! / 6 */
    seq_t *cat = (seq_t *)malloc((size_t)n * sizeof(seq_t)); /* Catalan(3,i) */
    for (idx_t i = 0; i < n; ++i) {
        seq_t f = factorial(i + 3);          /* (i+3)! */
        fac[i] = f / (seq_t)6;               /* divide by 3! */

        seq_t bin = binomial(2 * i + 3, i);
        seq_t tmp = seq_mul(bin, (seq_t)3);
        cat[i] = tmp / (seq_t)(2 * i + 3);   /* Catalan(3,i) */
    }

    /* compute the alternating sum */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = seq_mul(prev[3 + i], fac[i]);   /* S(n,3+i) * fac_div6 */
        term = seq_mul(term, cat[i]);               /* * Catalan(3,i) */
        if (i & 1)
            sum = sum - term;
        else
            sum = sum + term;
    }

    free(prev); free(curr); free(fac); free(cat);
    return sum;
}
