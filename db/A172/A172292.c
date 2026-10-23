/*
 * A172292  Triangle read by rows: T(n, k) = (2*n+1)*(2*k+1), n>=1, 1<=k<=n.
 * https://oeis.org/A172292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = (2*n+1)*(2*k+1).  The sequence a(n)
   consists of the terms of this triangle read row‑wise, with offset 1.
   For a given index n ≥ 1, the corresponding row r and column k are
   obtained from the triangular indexing and then
   a(n) = (2*r+1)*(2*k+1). */

seq_t a(idx_t n)
{
    if (n == 0)          /* not used because offset 1, but guard against bad input */
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n  (triangular numbers). */
    idx_t r = 0;
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) {
            r = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Compute the previous cumulative total before row r. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;   /* column index, 1‑based */

    /* Evaluate the formula (2*r+1)*(2*k+1). */
    seq_t part1 = (seq_t)r * 2 + 1;
    seq_t part2 = (seq_t)k * 2 + 1;
    return part1 * part2;
}
