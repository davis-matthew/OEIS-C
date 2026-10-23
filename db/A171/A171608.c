/*
 * A171608  Triangle by columns, T(n,k); (..., n, (n+1)) preceded by (n-1)
 *          zeros, as an infinite lower triangular matrix.
 * https://oeis.org/A171608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = floor(r/2)+1 if n lies at column floor((r+1)/2) of row r,
 *            where r is the unique integer with r(r-1)/2 < n ≤ r(r+1)/2;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1ull)
        return 0;

    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of elements preceding row r. */
    idx_t prev = (idx_t)(((__uint128_t)(r - 1) * r) / 2);

    idx_t pos    = n - prev;            /* 1‑based position in the row */
    idx_t target = (r + 1) / 2;          /* column where the non‑zero entry appears */

    if (pos == target)
        return (seq_t)(r / 2 + 1);        /* value in that cell */

    return 0;
}
