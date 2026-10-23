/*
 * A143976  Rectangular array R by antidiagonals: label each unit square in
 *          the first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares having x + y == 1 (mod 3); then R(m,n) is the number of
 *          UNmarked squares in the rectangle [0,m] X [0,n].
 * https://oeis.org/A143976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 755, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A143976: number of unmarked squares in rectangle [0,m]×[0,n] 
 * with x+y≡1 (mod 3), enumerated along antidiagonals.
 * For a pair (m,n) the value is  m*n - floor(m*n/3).
 * The n‑th term corresponds to the pair (m,n) in the antidiagonal order.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1, but guard for safety */

    /* Find diagonal d such that C(d-1) < n ≤ C(d),
       where C(d) = (d-1)*d/2 is the cumulative count up to diagonal d. */
    idx_t low = 1, high = 1;
    /* Find an upper bound for d */
    while ((unsigned __int128)(high - 1) * high / 2 < n) {
        high <<= 1;
    }
    /* Binary search for minimal d with C(d) ≥ n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t cnt = (unsigned __int128)(mid - 1) * mid / 2;
        if (cnt >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                          /* diagonal index (≥2) */

    /* Count of elements before this diagonal */
    idx_t prev = (unsigned __int128)(d - 2) * (d - 1) / 2;
    /* Position within the diagonal (0‑based) */
    idx_t pos = n - prev - 1;

    /* Coordinates along the diagonal: m = pos+1, n' = d - m */
    idx_t m  = pos + 1;
    idx_t n_ = d - m;

    /* Compute m*n - floor(m*n/3) */
    seq_t prod = (seq_t)m * (seq_t)n_;
    seq_t result = prod - prod / (seq_t)3;
    return result;
}
