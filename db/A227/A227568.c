/*
 * A227568  Largest k such that a partition of n into distinct parts with
 *          boundary size k exists.
 * https://oeis.org/A227568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*sqrt(n/3))
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by binary search: largest k with 3*k^2 <= 4*n   */

seq_t a(idx_t n)
{
    /* If n is zero, the answer is immediately zero. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Binary search over k in [0, n]  */
    idx_t lo = 0;
    idx_t hi = n + 1;                 /* hi is exclusive upper bound   */
    unsigned __int128 four_n = (unsigned __int128)4 * n;  /* 4*n, 128‑bit to avoid overflow */

    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * mid * 3;  /* 3*mid^2 */

        if (prod <= four_n)  /* 3*mid^2 <= 4*n  */
            lo = mid;
        else
            hi = mid - 1;
    }

    return (seq_t)lo;       /* lo satisfies 3*lo^2 <= 4*n and is maximal */
}
