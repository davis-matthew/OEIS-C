/*
 * A135841  A000012 * A135839 as infinite lower triangular matrices.
 * https://oeis.org/A135841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th entry (row-wise) of the lower–triangular * array
 *            T with * T(n,1) = n, * T(n,n) = 1, * T(n,k) = floor((n - k +
 *            2)/2) for 1 < k < n. * The sequence lists T(1,1), then
 *            T(2,1),T(2,2), then T(3,1),T(3,2),T(3,3), * and so on. For n ≥ 1
 *            this returns the required value
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for unused 0‑index */

    /* ---------- find the row r such that r*(r+1)/2 >= n ------------- */
    idx_t low  = 1;
    idx_t high = n;                       /* r*(r+1)/2 ≥ n for r≥n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit for the triangular calculation to avoid overflow */
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* ---------- compute column k ------------------------------------ */
    /* previous total terms up to row r-1 : (r-1)*r/2  */
    __int128 prev128 = (__int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;
    idx_t k = n - prev;    /* 1‑based column index */

    /* ---------- compute the entry value ----------------------------- */
    seq_t val;
    if (k == 1) {
        val = (seq_t)r;                       /* first column is n */
    } else {
        /* floor((r - k + 2)/2) */
        val = (seq_t)(r - k + 2) / 2;          /* integer division truncates */
    }
    return val;
}
