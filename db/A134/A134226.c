/*
 * A134226  Triangle T(n, k) = 3*n - 4 if k = n-1 otherwise k, read by rows.
 * https://oeis.org/A134226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle * T(r,k) = k (1 ≤ k ≤
 *            r), except T(r,r-1) = 3*r – 4. * The triangle is read row‑wise
 *            starting with r = 1. * Thus a(n) is obtained by locating the row
 *            r such that * r(r+1)/2 ≥ n and then taking the k-th entry in
 *            that row
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the first r with r*(r+1)/2 >= n. */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Number of terms in all previous rows. */
    idx_t prev = ((unsigned __int128)r * (r - 1)) / 2;

    /* Column index (1‑based) within row r. */
    idx_t k = n - prev;

    /* If k is the position r‑1, use the special value. */
    if (k == r - 1) {
        return (seq_t)(3 * r - 4);
    }
    return (seq_t)k;
}
