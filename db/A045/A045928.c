/*
 * A045928  The generalized Connell sequence C_{3,2}.
 * https://oeis.org/A045928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*n - 2*floor(1 + sqrt(n-1))
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* floor(1 + sqrt(n-1)) = floor(sqrt(n-1)) + 1, so we compute the integer
   square‑root of (n-1).  The function is defined for n >= 1; for n == 0
   it returns 0, but this is outside the defined offset. */

static inline idx_t isqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) – uses only 64‑bit arithmetic */
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;        // sqrt(UINT64_MAX) < 2^32
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;   // upper mid to avoid overflow
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* outside the offset – return 0 */
        return (seq_t)0;

    idx_t s = isqrt_u64(n - 1);   /* floor(sqrt(n-1)) */
    seq_t val = (seq_t)3 * (seq_t)n
             - (seq_t)2 * ((seq_t)s + (seq_t)1);
    return val;
}
