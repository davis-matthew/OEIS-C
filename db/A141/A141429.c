/*
 * A141429  Triangle T(n, k) = (k+1)*(n-k+1), read by rows.
 * https://oeis.org/A141429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) = (k+1)*(r-k+1) where r is the smallest positive
 *            integer with r(r+1)/2 >= n and k = n - (r-1)r/2 (the position of
 *            n in row r)
 * Range:     u8 n <= 274, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but safe guard */

    /* find r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < (seq_t)n) r++;

    /* k = n - (r-1)r/2 */
    idx_t prev_tri = (idx_t)(((seq_t)(r - 1) * r) / 2);
    idx_t k = n - prev_tri;

    return ((seq_t)(k + 1)) * ((seq_t)(r - k + 1));
}
