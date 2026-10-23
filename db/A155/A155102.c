/*
 * A155102  Triangle T(n,k) read by rows. If n=k then T(n,k)=1, elseif n=2*k
 *          then T(n,k)=-(k+1), else T(n,k)=0.
 * https://oeis.org/A155102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the infinite lower‑triangular matrix
 *            T(n,k) with T(n,k) = 1 if n == k = -(k+1) if n == 2*k = 0
 *            otherwise by rows, with row n containing the entries k = 1..n.
 *            For a given offset n (1‑based) we 1) find the row r such that
 *            r(r+1)/2 >= n and (r-1)r/2 < n 2) let k = n - (r-...
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* Find the smallest r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;              /* triangular(high) >= high >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms before this row */
    unsigned __int128 prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;            /* column index in this row (1‑based) */

    if (k == r)               return (seq_t)1;
    if (r == 2 * k) {
        seq_t m = (seq_t)(k + 1);
        return -m;
    }
    return (seq_t)0;
}
