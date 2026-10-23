/*
 * A173988  Triangle T(n,k) = 2^(k-1)*n*binomial(n-k,2*k-2)/(n-3*k+3) if
 *          k<n/3+1, else T(n,k)=1.
 * https://oeis.org/A173988
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = the n‑th term (in ascending order starting at n = 2) of
 *            the triangle T(n,k) defined by T(n,k) = 2^(k-1) * n *
 *            binomial(n-k, 2*k-2) / (n-3*k+3) if k ≤ floor((n+2)/3) 1
 *            otherwise where 1 ≤ k ≤ floor(n/2). The rows are concatenated
 *            row‑by‑row. The sequence starts with a(2) = 1
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;   /* undefined for n < 2; return 1 */

    /* Convert to 0‑based index within the concatenated list. */
    idx_t idx = n - 2;          /* idx = 0 corresponds to a(2) */

    /* Find the row r such that cumulative_count(r-1) <= idx < cumulative_count(r) */
    idx_t r = 2;
    for (;; ++r) {
        idx_t half1 = r / 2;
        idx_t half2 = r - half1;           /* ceil(r/2) */
        idx_t cum = half1 * half2;         /* total terms up to row r */
        if (cum > idx)
            break;
    }

    idx_t half1_prev = (r - 1) / 2;
    idx_t half2_prev = (r - 1) - half1_prev;
    idx_t cum_prev = half1_prev * half2_prev; /* total terms before row r */

    idx_t offset = idx - cum_prev;                /* 0‑based position within the row */
    idx_t k = offset + 1;                        /* column index (1‑based) */

    /* Determine whether the formula applies or entry is 1. */
    idx_t threshold = (r + 2) / 3;                /* floor((n+2)/3) */
    if (k > threshold)
        return (seq_t)1;

    /* 2^(k-1) */
    seq_t two_pow = pow2(k - 1);

    /* binomial(n-k, 2*k-2) */
    seq_t bin = binomial(r - k, 2 * k - 2);

    /* numerator = 2^(k-1) * n * bin */
    seq_t num = two_pow * (seq_t)r;
    num = num * bin;

    /* denominator = n - 3*k + 3   (positive by construction) */
    seq_t denom = (seq_t)(r - 3 * k + 3);

    /* Final value */
    return num / denom;
}
