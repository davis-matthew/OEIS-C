/*
 * A094727  Triangle read by rows: T(n,k) = n + k, 0 <= k < n, n >= 1.
 * https://oeis.org/A094727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = row + col where row = smallest integer such that
 *            row*(row+1)/2 ≥ n col = n – row*(row‑1)/2 – 1 The value is
 *            T(row, col) = row + col (This is equivalent to the OEIS formula
 *            a(n) = n – m(m‑3)/2 – 1 with m = floor((sqrt(8n‑7)+1)/2).)
 * Range:     u8 n <= 8383, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; for n==0 the behaviour is irrelevant. */
    if (n == 0) return 0;

    /* Binary search for the row r: r*(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;          /* r never exceeds n          */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;

    /* Number of elements before row r:  (r‑1)*r/2 */
    unsigned __int128 cum_prev = (unsigned __int128)(r - 1) * r / 2;

    /* Position within the row (0‑based) */
    idx_t k = (idx_t)(n - cum_prev - 1);

    /* The triangle entry: row + column */
    return (seq_t)r + (seq_t)k;
}
