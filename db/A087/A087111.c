/*
 * A087111  This table shows the coefficients of combinatorial formulas needed
 *          for generating the sequential sums of p-th powers of binomial
 *          coefficients C(n,7). The p-th row (p>=1) contains a(i,p) for i=1
 *          to 7*p-6, where a(i,p) satisfies Sum_{i=1..n} C(i+6,7)^p = 8 *
 *          C(n+7,8) * Sum_{i=1..7*p-6} a(i,p) * C(n...
 * https://oeis.org/A087111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 31, u64 n <= 63, u128 n <= 172,
 *            big any n
 * Helpers:   binom, term_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A087111
   a(n) is the n‑th entry of the table a(i,p) where **p** is the row
   number (p ≥ 1) and **i** is the index within that row, 1 ≤ i ≤ 7p-6.
   The entry is given by the combinatorial sum

      a(i,p) = Σ_{k=1}^{⌊(2i+1+(-1)^(i-1))/4⌋}
                 [ C(i-1, 2k-2) * C(i-2k+8, i-2k+1)^(p-1)
                 - C(i-1, 2k-1) * C(i-2k+7, i-2k)^(p-1) ]

   The implementation below uses the utility function `binom(a,b)` for
   binomial coefficients and `pow_seq_t` for exponentiation of `seq_t`
   values.  It computes the row index `p` and the position `i` from `n`
   by binary searching the cumulative term counts
   term_count(p) = (7p^2 - 5p)/2.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/term_count.h"

/* Helper: exponentiation of seq_t values (base^exp) */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    if (exp == 0) return res;
    if (base == (seq_t)0) return (seq_t)0;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

/* Main function: returns the n‑th term of A087111 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety; not used by tests */

    /* --- find the row index p such that term_count(p-1) < n ≤ term_count(p) --- */
    idx_t lo = 1, hi = 1;
    while (term_count(hi) < n) hi <<= 1;          /* exponential search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (term_count(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t p = lo;                     /* row number (1‑based) */

    /* index of i within that row (1‑based) */
    idx_t prev_terms = (p == 1) ? 0 : term_count(p - 1);
    idx_t idx_within = n - prev_terms - 1;   /* 0‑based */
    idx_t i = idx_within + 1;                /* 1‑based */

    /* --- compute a(i,p) by the explicit combinatorial formula --- */
    idx_t numerator = 2 * i + 1;
    /* add (-1)^(i-1) which equals 1 if i is odd, -1 if i is even */
    numerator += (i & 1) ? 1 : -1;
    idx_t kmax = numerator / 4;             /* floor division gives the limit */

    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t t1 = (seq_t)0;
        seq_t t2 = (seq_t)0;

        /* first term: C(i-1, 2k-2) * C(i-2k+8, i-2k+1)^(p-1) */
        if (i >= 1 && (i - 1) >= 2 * k - 2) {
            t1 = binom(i - 1, 2 * k - 2);
            idx_t n1 = i - 2 * k + 8;
            idx_t r1 = i - 2 * k + 1;
            if (n1 >= r1 && r1 >= 0) {
                seq_t base1 = binom(n1, r1);
                seq_t pow1  = pow_seq_t(base1, p - 1);
                t1 *= pow1;
            }
        }

        /* second term: C(i-1, 2k-1) * C(i-2k+7, i-2k)^(p-1) */
        if (i >= 1 && (i - 1) >= 2 * k - 1) {
            t2 = binom(i - 1, 2 * k - 1);
            idx_t n2 = i - 2 * k + 7;
            idx_t r2 = i - 2 * k;
            if (n2 >= r2 && r2 >= 0) {
                seq_t base2 = binom(n2, r2);
                seq_t pow2  = pow_seq_t(base2, p - 1);
                t2 *= pow2;
            }
        }

        result += t1;
        result -= t2;
    }
    return result;
}
