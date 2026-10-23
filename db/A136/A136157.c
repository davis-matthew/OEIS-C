/*
 * A136157  Triangle by columns, (3, 1, 0, 0, 0, ...) in every column.
 * https://oeis.org/A136157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3 if k==r, 1 if k==r-1, else 0; where r is the unique row
 *            with t[r] <= n < t[r+1], k = n - t[r], and t[r] = r(r+1)/2
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t low  = 0;
    idx_t high = (n < 0xFFFFFFFFULL) ? n : 0xFFFFFFFFULL;
    idx_t best = 0;

    /* binary search for the largest r such that r(r+1)/2 <= n */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tmid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tmid <= n) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    idx_t r  = best;
    unsigned __int128 t  = (unsigned __int128)r * (r + 1) / 2;
    idx_t      k  = n - (idx_t)t;

    if (k == r)                 return (seq_t)3;
    else if (r > 0 && k == r-1) return (seq_t)1;
    else                         return (seq_t)0;
}
