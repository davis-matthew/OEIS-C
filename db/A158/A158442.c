/*
 * A158442  Triangle T(n,k) = [x^k] n!*(n+1+x^n)*Sum_{i=0..n-1} x^i/(i+1).
 * https://oeis.org/A158442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the (n‑th) entry of the triangle T(r,k) defined as the
 *            coefficient of x^k in r! * ( r+1 + x^r ) * Σ_{i=0}^{r-1}
 *            x^i/(i+1) where r is the unique integer with r*(r+1) ≥ n and
 *            (r−1)*r < n. For k < r : a(n) = r! * (r+1) / (k+1) For k ≥ r :
 *            a(n) = r! / (k−r+1)
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root: floor(sqrt(n)) using 128‑bit arithmetic. */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* hi > sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1) >= n > (r-1)*r */
    idx_t s = _isqrt_u64((idx_t)1 + (idx_t)4 * n);   /* floor(sqrt(1+4n)) */
    idx_t r = (s - (idx_t)1) / (idx_t)2;             /* floor of root */
    while ((unsigned __int128)r * (r + (idx_t)1) < n)
        ++r;                                        /* make r minimal */

    /* Compute the 0‑based column index k within the row */
    idx_t offset = (r - (idx_t)1) * r;              /* terms before row r */
    idx_t k = n - offset - (idx_t)1;                /* column in [0,2r-1] */

    /* Compute r! in seq_t */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= r; ++i)
        fact *= (seq_t)i;

    /* Evaluate the entry */
    if (k < r) {
        seq_t numerator = fact * ((seq_t)r + (seq_t)1);   /* r! * (r+1) */
        seq_t denominator = (seq_t)(k + (idx_t)1);       /* k+1 */
        return numerator / denominator;                 /* exact division */
    } else {
        seq_t denominator = (seq_t)(k - r + (idx_t)1);    /* k-r+1 */
        return fact / denominator;                       /* exact division */
    }
}
