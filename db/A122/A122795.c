/*
 * A122795  Connell (5,3)-sum sequence (partial sums of the (5,3)-Connell
 *          sequence).
 * https://oeis.org/A122795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^2 - 3*n)/2 - 4*Σ_{k=1..n}
 *            floor((1+floor(sqrt(24*k-23)))/6)
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Connell (5,3)-sum sequence A122795. */

static inline idx_t isqrt_u64(idx_t x)
{
    /* Integer floor square root of a 64‑bit nonnegative integer. */
    idx_t lo = 0, hi = x;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__int128)mid * mid) <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* value below the offset is irrelevant */

    unsigned long long sumR = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t t = 24 * k - 23;
        idx_t rt = isqrt_u64(t);          /* floor(sqrt(t)) */
        idx_t R  = (1 + rt) / 6;          /* floor((1+rt)/6) */
        sumR += (unsigned long long)R;
    }

    unsigned long long nn = (unsigned long long)n;
    unsigned long long aval = (5ULL * nn * nn - 3ULL * nn) / 2ULL
                               - 4ULL * sumR;

    return (seq_t)aval;
}
