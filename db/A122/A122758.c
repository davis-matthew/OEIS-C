/*
 * A122758  Triangle read by rows: T(n,m) = 2*n^2*A084221(n) (n>=0, 0 <= m <=
 *          n).
 * https://oeis.org/A122758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle read by rows where
 *            T(row,col) = 2 * row^2 * g(col) with g(col) defined by g(2k) =
 *            4^k g(2k+1) = 3 * 4^k (k >= 0) The row index starts at 0 and the
 *            column index runs from 0 to row inclusive. The sequence is
 *            flattened: first the row 0 element, then all row 1...
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute 4^p for p >= 0 using repeated multiplication. */
static seq_t pow4(unsigned long long p) {
    seq_t res = 1;
    for (unsigned long long i = 0; i < p; ++i) res *= 4;
    return res;
}

/* Helper: find the row r (0‑based) such that indices up to r inclusive reach n.
   The total number of entries up to row r is (r+1)(r+2)/2.  We need the smallest r
   with (r+1)(r+2)/2 >= n.  Since n is 1‑based we work with idx_t throughout. */
static idx_t find_row(idx_t n) {
    /* Binary search on t = r+1, so we search for smallest t with t(t+1)/2 >= n */
    idx_t low = 0, high = 1;
    while ((seq_t)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high > n) { high = n + 1; break; } /* avoid overflow */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) / 2 >= n) high = mid;
        else low = mid;
    }
    return high - 1; /* r = t-1 */
}

/* Main function: compute a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for accidental n==0 */
    /* Determine the row r (0‑based) and column c (0‑based). */
    idx_t r = find_row(n);                  /* r satisfies (r+1)(r+2)/2 >= n */
    idx_t row_start = r * (r + 1) / 2;      /* entries before this row, 0‑based */
    idx_t c = n - row_start - 1;            /* column within the row */

    /* Compute g(c). */
    unsigned long long k = c >> 1;          /* floor(c/2) */
    seq_t g = pow4(k);
    if (c & 1ULL) g *= 3;                    /* multiply by 3 if c is odd */

    /* Compute 2 * r^2 * g.  Use seq_t for all arithmetic. */
    seq_t row_val = (seq_t)r * (seq_t)r;     /* r^2 */
    row_val *= 2;                            /* 2*r^2 */
    row_val *= g;                            /* 2*r^2*g */
    return row_val;
}
