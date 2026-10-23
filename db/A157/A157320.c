/*
 * A157320  Symmetrical Hahn weights on q-form factorials:m=1;q=2;
 *          q-form:t(n,m)=If[m == 0, n!, Product[Sum[(m + 1)^i, {i, 0, k -
 *          1}], {k, 1, n}]]; Hahn weight:b(n,k,m)=If[n == 0, 1, (n!*t[m + 1,
 *          k]*t[m + 1, n - k])/(k!*(n - k)!*t[1, n])].
 * https://oeis.org/A157320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, uned
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A157320 – Symmetrical Hahn weights (m = 1, q = 2)
 *
 *  For n > 0 let r be the unique integer with
 *
 *        r*(r+1)/2  ≤  n  <  (r+1)*(r+2)/2
 *
 *  and let k = n - r*(r+1)/2   (0 ≤ k ≤ r).  
 *
 *  Define
 *
 *        t₂(0) = 2
 *        t₂(x) = x + 2        for x ≥ 1
 *
 *  Then the sequence term is
 *
 *        a(0) = 1
 *        a(n) = C(r,k) * t₂(k) * t₂(r-k)            for n > 0
 *
 *  where C(r,k) is the binomial coefficient.  This follows directly
 *  from the definition of the Hahn weights with m = 1 and q = 2.
 *  The function below implements this formula using only integer
 *  arithmetic and the operations defined for the arbitrary‑precision
 *  type seq_t.
 */
#include <stddef.h>          /* for size_t */

static idx_t
find_row(idx_t n)
{
    /* Find largest r such that r*(r+1)/2 <= n.
       Binary search – works for any n in the 0‑based range. */
    idx_t low = 0, high = n * 2 + 2;     /* a safe upper bound */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        idx_t cum = mid * (mid + 1) / 2;
        if (cum <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t
binomial_nk(idx_t n, idx_t k)
{
    /* Compute C(n,k) multiplicatively.
       Use symmetry to keep the loop small. */
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use smaller k */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (n - k + i) / i; division is exact */
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */

    /* Determine row r and column k */
    idx_t r = find_row(n);
    idx_t k = n - r * (r + 1) / 2;   /* 0 ≤ k ≤ r */

    /* Compute t₂(k) and t₂(r-k) */
    seq_t t2k = (k == 0) ? (seq_t)2 : (seq_t)(k + 2);
    seq_t t2rk = (r == k) ? (seq_t)2 : (seq_t)( (r - k) + 2 );

    /* Binomial coefficient C(r,k) */
    seq_t bin = binomial_nk(r, k);

    /* Final result */
    return bin * t2k * t2rk;
}
