/*
 * A130154  Triangle read by rows: T(n, k) = 1 + 2*(n-k)*(k-1) (1 <= k <= n).
 * https://oeis.org/A130154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + 2*(r - k)*(k - 1), where r is the smallest integer
 *            with r(r+1)/2 ≥ n, and k = n - r(r-1)/2 (row and column indices
 *            of the triangle)
 * Range:     u8 n <= 286, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset starts at 1; for safety return 0 if called with n==0 */
    if (n == 0) return (seq_t)0;

    /* Binary search for r: 1 ≤ r ≤ 2^32   (covers all n ≤ 2^64‑1) */
    idx_t low  = 1;
    idx_t high = (idx_t)4294967296ULL;  /* 2^32 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;  /* row index */

    /* Number of terms before row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;  /* column index, 1-based */

    seq_t rs = (seq_t)r;
    seq_t ks = (seq_t)k;

    return (seq_t)1 + ( (seq_t)2 * (rs - ks) * (ks - (seq_t)1) );
}
