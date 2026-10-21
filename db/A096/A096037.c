/*
 * A096037  Triangle T(n,m) = (3*n+3*m-2)*(n+1-m)/2 read by rows.
 * https://oeis.org/A096037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = (3*r+3*c-2)*(r+1-c)/2 and r is the
 *            row number such that (r-1)*r/2 < n <= r*(r+1)/2, c = n -
 *            (r-1)*r/2. The triangle is read by rows
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the row index r for a given linear index n (offset 1).  */
static idx_t row_of(idx_t n) {
    /* Find smallest r with r*(r+1)/2 >= n.  */
    /* Use binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0;
    idx_t high = 1;
    while ( (unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1; /* double until condition holds */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Determine row r and column c from linear index n.  */
    idx_t r = row_of(n);                       /* 1‑based */
    idx_t prev_terms = (r - 1) * r / 2;        /* terms before row r */
    idx_t c = n - prev_terms;                  /* 1‑based column in row r */

    /* Compute T(r,c) using seq_t arithmetic only.  */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)c;
    seq_t dr = (seq_t)3 * sr + (seq_t)3 * sc - (seq_t)2;
    seq_t dc = sr + (seq_t)1 - sc;
    return dr * dc / (seq_t)2;
}
