/*
 * A101372  Triangle read by rows: T(n,k) is number of leaves at level k in
 *          all noncrossing rooted trees on n+1 nodes.
 * https://oeis.org/A101372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 1, let r be the unique integer with r(r+1)/2 ≥ n. Set k
 *            = n – r(r–1)/2. Then a(n) = T(r,k), where T(r,k) = 2^(k–1) *
 *            ((3k–1)/(2r+k–1)) * binomial(3r–2, r–k) (1 ≤ k ≤ r) The binomial
 *            and powers are computed exactly
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom, integer_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/integer_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0, return 0 */

    /* Find row r such that r(r+1)/2 ≥ n */
    idx_t r = (integer_sqrt(1 + 8*n) - 1) / 2;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev_sum = r * (r - 1) / 2;            /* sum of first r–1 terms */
    idx_t k = n - prev_sum;                     /* column index (1‑based) */

    /* Compute T(r,k) using the explicit formula */
    seq_t pow   = pow2(k - 1);                   /* 2^(k–1) */
    seq_t bin   = binom(3 * r - 2, r - k);       /* binomial(3r–2, r–k) */
    seq_t num   = (seq_t)(3 * k - 1);             /* (3k–1) */
    seq_t den   = (seq_t)(2 * r + k - 1);         /* (2r+k–1) */

    seq_t res = bin * num;                       /* multiply first */
    res /= den;                                  /* exact division */
    res *= pow;                                  /* final multiply */

    return res;
}
