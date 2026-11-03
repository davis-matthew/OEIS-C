/*
 * A057105  Triangle of numbers (when unsigned) related to congruum problem:
 *          T(n,k)=k^2+2nk-n^2 with n>k>0 and starting at T(2,1)=1.
 * https://oeis.org/A057105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(n,k) where T(n,k) = k^2 + 2*n*k - n^2 The sequence is
 *            obtained by row‑major enumeration of the triangle for n =
 *            2,3,4,… and k = 1,…,n‑1
 * Range:     i8 n <= 44, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the row index (n) for a given position in the flattened triangle. */
static idx_t row_for_index(idx_t idx) {
    idx_t low = 2;
    idx_t high = 2;
    /* find an upper bound */
    while ((unsigned __int128)high * (high - 1) / 2 < idx) high <<= 1;
    /* binary search for the smallest n with n*(n-1)/2 >= idx */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset 1: a(1) is the first term */
    idx_t idx = n;                  /* convert to idx_t for safety */
    idx_t row = row_for_index(idx);

    /* cumulative terms up to the previous row: (row-2)*(row-1)/2 */
    unsigned __int128 prev128 = (unsigned __int128)(row - 2) * (row - 1) / 2;
    idx_t prev = (idx_t)prev128;

    idx_t k = idx - prev;           /* 1‑based column within the row */

    /* compute T(row, k) = k^2 + 2*row*k - row^2 */
    seq_t N = (seq_t)row;
    seq_t K = (seq_t)k;
    seq_t result = K * K + 2 * N * K - N * N;
    return result;
}
