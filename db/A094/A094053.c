/*
 * A094053  Triangle read by rows: T(n,k) = k*(n-k), 1 <= k <= n.
 * https://oeis.org/A094053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k*(n-k) where (n,k) are the row and column indices
 *            obtained from the triangular enumeration of the numbers k*(n-k),
 *            1≤k≤n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* find row r and column k for the term at linear index n   (n ≥ 1)   */
static void _row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t low  = 0;
    idx_t high = 1;

    /*  double high until the cumulative count  high*(high+1)/2  exceeds n  */
    while (((unsigned __int128)high * (high + 1)) / 2 < n) {
        /* high < n, so the product fits in 128 bits */
        high <<= 1;
    }

    /* binary search in [low+1, high] */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (cum >= n)
            high = mid;
        else
            low  = mid;
    }

    *row = high;
    /* elements before this row */
    unsigned __int128 prev = ((unsigned __int128)(high - 1) * high) / 2;
    *col = n - (idx_t)prev;      /* col is 1‑based, 1 ≤ col ≤ row   */
}

/* Sequence A094053 */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1 only   */
        return 0;

    idx_t r, k;
    _row_col(n, &r, &k);

    /* compute k*(r-k) in the target integer type */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    return sk * (sr - sk);
}
