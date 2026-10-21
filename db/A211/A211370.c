/*
 * A211370  Array read by antidiagonals: T(m,n) = Sum( n <= i <= m+n-1 ) i!.
 * https://oeis.org/A211370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Sum_{i = ncol}^{s-1} i! where s is the smallest integer
 *            such that (s-1)*s/2 ≥ n, and ncol = n - (s-2)*(s-1)/2. (Indicies
 *            start at 1.)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root, floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          // hi² ≥ n for 64‑bit n
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)            return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Helper: triangular number T(s) = (s-1)*s/2.  Return as idx_t. */
static idx_t triangular(idx_t s)
{
    return (idx_t)((unsigned __int128)(s - 1) * s / 2);
}

/* Main function for the sequence. */
seq_t a(idx_t n)
{
    /* Determine the antidiagonal (s).  s satisfies T(s) ≥ n. */
    idx_t s = (1 + isqrt_u64(1 + 8 * (unsigned long long)n)) / 2;
    /* Adjust if the triangular number is still too small. */
    while (triangular(s) < n) {
        ++s;
    }

    /* Number of terms before diagonal s.  = T(s-1). */
    idx_t prev = triangular(s - 1);
    /* Position within this diagonal (1‑based). */
    idx_t offset = n - prev;          /* 1 ≤ offset ≤ s-1 */
    idx_t ncol = s - offset;          /* column index (>=1) */

    /* Compute sum of factorials from ncol to s-1. */
    seq_t sum = 0;
    seq_t fact = 1;
    for (idx_t i = 1; i < s; ++i) {
        fact *= (seq_t)i;             /* i! */
        if (i >= ncol) {
            sum += fact;
        }
    }
    return sum;
}
