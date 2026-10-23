/*
 * A176283  Triangle T(n,k) = 1 + A000537(n) - A000537(k) - A000537(n-k), read
 *          by rows.
 * https://oeis.org/A176283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) * where r is the unique row index such that
 *            r(r+1)/2 <= n < (r+1)(r+2)/2, * k = n - r(r+1)/2, and * T(r,k) =
 *            1 + s_r^2 - s_k^2 - s_{r-k}^2, * with s_m = m(m+1)/2. * The
 *            function evaluates this with only integer arithmetic and no *
 *            assumptions about the ordering of calls
 * Range:     u8 n <= 22, u16 n <= 286, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t _find_row(idx_t n)
{
    idx_t low = 0, high = n;           /* [low, high] is the search interval */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        /* compute mid*(mid+1)/2 using 128-bit to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        idx_t sum = (idx_t)(prod / 2);
        if (sum <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r     = _find_row(n);
    idx_t offs  = r * (r + 1) / 2;      /* offset where row r starts */
    idx_t k     = n - offs;             /* column within the row */

    /* s_m = m(m+1)/2 evaluated in seq_t */
    seq_t sr = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t sk = (seq_t)k * ((seq_t)k + 1) / 2;
    seq_t snk = (seq_t)(r - k) * ((seq_t)(r - k) + 1) / 2;

    /* T(r,k) = 1 + sr^2 - sk^2 - snk^2 */
    seq_t result = (seq_t)1 + sr * sr - sk * sk - snk * snk;
    return result;
}
