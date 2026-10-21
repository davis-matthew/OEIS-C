/*
 * A086873  Triangle read by rows in which row n >= 1 gives coefficients in
 *          expansion of the polynomial Sum_{k=1..n} (1/n) * binomial(n,k) *
 *          binomial(n,k-1) * x^(2k) * (1+x)^(2n-2k) / x^2 in powers of x.
 * https://oeis.org/A086873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, changed
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A086873
   The N-th term is the N-th element (offset 1) of the triangle whose
   r‑th row (r ≥ 1) contains 2r−1 coefficients given by

        a_{r,m} = (1/r) · Σ_{k=1..r} C(r,k)·C(r,k‑1)·C(2r‑2k, m‑2k+2),
        0 ≤ m ≤ 2r‑2 ,

   where C(n,k) denotes the binomial coefficient.
   The flattened index satisfies
        (r‑1)² < n ≤ r² ,
   and the intra‑row position is
        p = n – (r‑1)² – 1   (0‑based, 0 ≤ p < 2r‑1).
   The implementation below uses this closed‑form formula, with
   helper routines that work for any seq_t defined by the build. */

#include <stdint.h>

/* Compute C(n,k) multiplicatively using seq_t arithmetic.           */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry                     */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - i + 1);
        r = r / i;                     /* exact division                   */
    }
    return r;
}

/* Integer square‑root for idx_t: floor(sqrt(n)) using 128‑bit math. */
static idx_t isqrt128(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n) return mid;
        if (sq < (unsigned __int128)n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;                          /* floor sqrt */
}

/* Main sequence generator.  External linkage, no static data.       */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* offset is 1; undefined below */

    /* Determine row r : smallest r with r² ≥ n. */
    idx_t r = isqrt128(n);
    if (r * r < n) r++;

    /* Position p in the row, 0‑based.                     */
    idx_t prev_sum = (r - 1) * (r - 1);
    idx_t p = n - prev_sum - 1;           /* 0 ≤ p ≤ 2r‑2 */

    seq_t numerator = 0;

    for (idx_t k = 1; k <= r; ++k) {
        idx_t base_exp = 2 * k - 2;          /* exponent contributed by x^(2k) */
        if (p < base_exp) continue;          /* m too small for this k */

        idx_t j   = p - base_exp;           /* index for C(2r-2k, j)           */
        idx_t maxj = 2 * r - 2 * k;
        if (j > maxj) continue;              /* exponent too large              */

        seq_t term = binom(r, k);
        term = term * binom(r, k - 1);
        term = term * binom(maxj, j);
        numerator = numerator + term;
    }

    seq_t result = numerator / r;          /* division exact by construction   */
    return result;
}
