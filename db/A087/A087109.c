/*
 * A087109  This table shows the coefficients of combinatorial formulas needed
 *          for generating the sequential sums of p-th powers of binomial
 *          coefficients C(n,5). The p-th row (p>=1) contains a(i,p) for i=1
 *          to 5*p-4, where a(i,p) satisfies Sum_{i=1..n} C(i+4,5)^p = 6 *
 *          C(n+5,6) * Sum_{i=1..5*p-4} a(i,p) * C(n...
 * https://oeis.org/A087109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(i,p) = Sum_{k=1..[(2*i+1+(-1)^(i-1))/4]} [ *
 *            C(i-1,2k-2)*C(i-2k+6,i-2k+1)^(p-1) *
 *            -C(i-1,2k-1)*C(i-2k+5,i-2k)^(p-1) ] * The sequence a(n) is the
 *            row‑major enumeration of the table * (row index = p ≥ 1, column
 *            index = i = 1 … 5p-4), * with offset 1 (a(1) is the first
 *            element of the first row)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 40, u64 n <= 90, u128 n <= 206,
 *            big any n
 * Helpers:   binom, pow_seq_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"          /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/pow_seq_int.h"

/* a(i,p) according to the OEIS formula */
static seq_t a_ip(idx_t p, idx_t i)
{
    seq_t sum = 0;
    /* number of summands in the formula; (i+1)/2 is equal to the
       closed form for all i ≥ 1. */
    idx_t kmax = (i + 1) / 2;
    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* Term 1: requires i ≥ 2*k-1 */
        if (i >= 2 * k - 1) {
            idx_t j1   = 2 * k - 2;                   /* binomial index for C(i-1,…) */
            seq_t c1   = binom(i - 1, j1);

            idx_t n1   = i - 2 * k + 6;               /* upper index for C(…,…)   */
            idx_t r1   = i - 2 * k + 1;               /* lower index for C(…,…)   */
            seq_t coeff1 = binom(n1, r1);
            seq_t pow1 = pow_seq_int(coeff1, p - 1);
            sum += c1 * pow1;
        }

        /* Term 2: requires i ≥ 2*k */
        if (i >= 2 * k) {
            idx_t j2   = 2 * k - 1;
            seq_t c2   = binom(i - 1, j2);

            idx_t n2   = i - 2 * k + 5;
            idx_t r2   = i - 2 * k;
            seq_t coeff2 = binom(n2, r2);
            seq_t pow2 = pow_seq_int(coeff2, p - 1);
            sum -= c2 * pow2;
        }
    }
    return sum;
}

/* cache for memoisation: cache[n-1] holds a(n) */
static seq_t *cache      = NULL;
static size_t cached_cnt = 0;   /* number of values already computed */
static size_t cached_cap = 0;   /* allocated capacity */

/* current position in the triangular enumeration
   (row = cur_row, column = cur_col) */
static idx_t cur_row = 1;   /* p   */
static idx_t cur_col = 1;   /* i   */

/* -------------  public function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset starts at 1; a(0) is undefined */
        return 0;

    /* grow cache if necessary */
    if (n > cached_cnt) {
        if (cached_cap < (size_t)n) {
            size_t newcap = (cached_cap ? cached_cap * 2 : 16);
            while (newcap < (size_t)n)
                newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cached_cap = newcap;
        }

        /* generate terms until we reach n */
        while (cached_cnt < n) {
            seq_t val = a_ip(cur_row, cur_col);
            cache[cached_cnt++] = val;

            /* advance to next position in the triangular indexing */
            ++cur_col;
            if (cur_col > 5 * cur_row - 4) {
                cur_col = 1;
                ++cur_row;
            }
        }
    }
    return cache[n - 1];
}
