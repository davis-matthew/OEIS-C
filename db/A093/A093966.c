/*
 * A093966  Array read by antidiagonals: number of {112,221}-avoiding words.
 * https://oeis.org/A093966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 97, u32 n <= 264, u64 n <= 722,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by reading the array A(n,k) (number of n‑long
   k‑ary words that avoid the patterns 112 and 221) along antidiagonals.
   For n,k ≥ 1:
       if k == 1          : A(n,1) = n
       if k > n           : A(n,k) = Σ_{j=1..n}  j·j!·C(n,j)
       else (2 ≤ k ≤ n)   : A(n,k) = k!·C(n,k) + Σ_{j=1..k-1}  j·j!·C(n,j)
   where C(n,j) is the binomial coefficient (computed by the supplied
   routine binom). The term a(n) is the nth element of the sequence
   obtained by anti‑diagonal traversal of this array. */

#include "../utils/binom.h"

static seq_t A_nk(idx_t n, idx_t k)
{
    /* k == 1: simple case */
    if (k == 1) {
        return (seq_t)n;
    }

    /* k > n: sum over all j */
    if (k > n) {
        seq_t sum  = 0;
        seq_t fact = 1;                      /* j!   */
        for (idx_t j = 1; j <= n; ++j) {
            fact *= j;                       /* fact = j!   */
            sum += (seq_t)j * fact * binom(n, j);
        }
        return sum;
    }

    /* 2 ≤ k ≤ n: split the sum */
    seq_t result = 0;
    seq_t fact   = 1;                          /* j!   */
    for (idx_t j = 1; j < k; ++j) {
        fact *= j;                             /* fact = j!   */
        result += (seq_t)j * fact * binom(n, j);
    }
    fact *= k;                                 /* fact = k!   */
    result += fact * binom(n, k);
    return result;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;            /* number of terms already accounted for */
    for (idx_t s = 2;; ++s) {
        idx_t elems = s - 1;  /* diagonal size */
        if (cum + elems >= n) {
            idx_t pos  = n - cum - 1;        /* zero‑based index within diagonal */
            idx_t row  = pos + 1;            /* n in the array   */
            idx_t col  = s - row;            /* k in the array   */
            return A_nk(row, col);
        }
        cum += elems;
    }
}
