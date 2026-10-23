/*
 * A115292  Correlation triangle for (1+x)^3/(1-x).
 * https://oeis.org/A115292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Correlation triangle for (1+x)^3/(1-x).
 *
 * The triangle T(n,k) is defined by  
 *    T(n,k) = Σ_{j=0}^{min(k,n-k)} A115291(k-j) * A115291(n-k-j)   (0 ≤ k ≤ n)
 * where A115291 is the sequence of coefficients of (1+x)^3/(1-x):
 *      A115291(i) = { 1, if i=0
 *                    { 4, if i=1
 *                    { 7, if i=2
 *                    { 8, otherwise.
 *
 * The OEIS sequence A115292 lists the entries of this triangle row‑major
 * (row 0 first, then row 1, …).  The following implementation computes
 * a(n) directly from its (row,column) coordinates without building the
 * entire triangle.
 */

static inline seq_t val_of(idx_t i) {
    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)4;
    if (i == 2) return (seq_t)7;
    return (seq_t)8;
}

/* Find the row r containing the linear index p (0‑based). */
static inline idx_t row_of(idx_t p) {
    /* Binary search for the smallest r such that (r+1)(r+2)/2 > p. */
    idx_t lo = 0, hi = 1;
    while (( (__uint128_t)hi * (hi + 1) ) / 2 <= p) {
        hi <<= 1;                 /* grow hi until the bound is crossed */
    }
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 <= p ) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo - 1;                /* lo is first exceeding r, so row is lo-1 */
}

/* Compute triangle entry T(n,k). */
static inline seq_t triangle_entry(idx_t n, idx_t k) {
    idx_t maxj = (k < n - k) ? k : n - k;
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t a1 = val_of(k - j);
        seq_t a2 = val_of(n - k - j);
        sum += a1 * a2;
    }
    return sum;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    idx_t r = row_of(n);
    idx_t c = n - r * (r + 1) / 2;
    return triangle_entry(r, c);
}
