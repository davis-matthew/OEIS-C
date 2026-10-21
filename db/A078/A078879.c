/*
 * A078879  Triangle read by rows: T(n,k) = ceiling(k^2 / n), 0 < k < n and
 *          T(n,n) = 0.
 * https://oeis.org/A078879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = row index and k = position in row row r
 *            consists of k = 1..r-1 values T(r,k) = ceil(k^2 / r), and a
 *            trailing 0. a(t) is the element obtained by reading this
 *            triangle row‑wise
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t tri = 1;               /* triangular number for current r */
    while (tri < n) {
        ++r;
        tri += r;
    }
    /* Number of terms before this row: prev_tri = r(r-1)/2 */
    idx_t prev_tri = tri - r;
    /* Position inside the row (1‑based). */
    idx_t pos = n - prev_tri;    /* 1 ≤ pos ≤ r */

    /* The last element of every row is 0. */
    if (pos == r) {
        return (seq_t)0;
    }

    /* Compute ceil(pos^2 / r) using a 64‑bit intermediate to avoid overflow
       on implementations with small seq_t types. */
    unsigned long long k = (unsigned long long)pos;
    unsigned long long rr = (unsigned long long)r;
    unsigned long long val = (k * k + rr - 1) / rr;  /* ceil division */

    return (seq_t)val;
}
