/*
 * A152420  Irregular triangle read by rows: T(n,k) = n*(n-2) - (k-n)*(k-n-2),
 *          with 0 <= k <= 2*n.
 * https://oeis.org/A152420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = T(r,k) where r = floor(sqrt(n)), k = n - r², T(n,k) =
 *            n*(n-2) - (k-n)*(k-n-2)
 * Range:     i8 n <= 178, i16 n <= 2600, i32 n <= 2600, i64 n <= 2600,
 *            i128 n <= 2600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1) << 32;          /* 2³² > sqrt(UINT64_MAX) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Avoid overflow: check mid*mid <= n via mid <= n/mid. */
        if (mid > 0 && mid > n / mid)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);       /* row index */
    idx_t k = n - r * r;          /* column offset in the row, 0 ≤ k ≤ 2*r */

    seq_t sr = (seq_t)r;          /* cast to seq_t for arithmetic */
    seq_t sk = (seq_t)k;

    seq_t term1 = sr * (sr - 2);  /* n*(n-2) */
    seq_t diff   = sk - sr;       /* k-n */
    seq_t term2  = diff * (diff - 2); /* (k-n)*(k-n-2) */

    return term1 - term2;
}
