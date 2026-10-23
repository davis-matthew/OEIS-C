/*
 * A162608  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n!, such that the difference between successive terms is also
 *          equal to n!.
 * https://oeis.org/A162608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) = r! * k, with r = floor((√(8n+1)-1)/2) and k = n
 *            - r(r+1)/2 + 1. The triangle is read row by row; the first row
 *            (r = 0) has 1 term, the second row 2 terms, etc
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 85, u64 n <= 216,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root of a 64‑bit unsigned integer, truncated down. */
static inline idx_t _isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, ans = 0;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= v)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* r = floor((√(8n+1)-1)/2) */
    idx_t r = (_isqrt_u64(8ULL * n + 1ULL) - 1ULL) / 2ULL;

    /* k = 1‑based column within row r */
    idx_t prev_terms = r * (r + 1ULL) / 2ULL;  /* terms before row r */
    idx_t k = n - prev_terms + 1ULL;

    /* compute r! in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i)
        fact *= (seq_t)i;

    return fact * (seq_t)k;
}
