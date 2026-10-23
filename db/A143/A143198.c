/*
 * A143198  Triangle read by rows T(n, k) = n + (n+1)*(k-1)*(k+2)/2.
 * https://oeis.org/A143198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the flattened triangle read by rows T(r,k) = r +
 *            (r+1)*(k-1)*(k+2)/2, where r = floor((sqrt(1+8n)-1)/2) and k = n
 *            - r*(r+1)/2
 * Range:     i8 n <= 26, i16 n <= 859, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses binary search to find r, avoiding floating point. */

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t low = 0;
    idx_t high = n + 1;          /* upper bound is n+1 (offset of next row ) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* k = column index within row r */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;

    /* Compute T(r,k) using seq_t arithmetic */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t term = rr + ((rr + (seq_t)1) * (kk - (seq_t)1) * (kk + (seq_t)2)) / (seq_t)2;
    return term;
}
