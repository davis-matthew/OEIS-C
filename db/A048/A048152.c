/*
 * A048152  Triangular array T read by rows: T(n,k) = k^2 mod n, for 1 <= k <=
 *          n, n >= 1.
 * https://oeis.org/A048152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 845, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048152:
   a(n) = (k^2) mod r, where n = r(r-1)/2 + k  (1 ≤ k ≤ r) 
   i.e. the nth element of the triangular array T(r,k) = k^2 mod r
   read by rows starting with n = 1 */
seq_t a(idx_t n) {
    idx_t r = 0;
    idx_t t = 0;          /* t = r(r+1)/2, the last index of row r   */
    while (t < n) {       /* find smallest r with t >= n             */
        r++;
        t += r;
    }
    idx_t t_prev = t - r; /* t_prev = (r-1)*r/2                      */
    idx_t k = n - t_prev; /* 1‑based column index within the row     */
    seq_t prod = (seq_t)k * k;
    return prod % (seq_t)r;
}
