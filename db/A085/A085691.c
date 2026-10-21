/*
 * A085691  Triangle read by rows: T(n,k) is the number of triangles of side k
 *          in triangular matchstick arrangement of side n; n>=1 and k>=1.
 * https://oeis.org/A085691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by flattening the rows of the triangle T(n,k)
 *            of side n in a triangular matchstick arrangement. For n≥1, let r
 *            be the smallest integer with r(r+1)/2 ≥ n, let c = n - (r-1)r/2.
 *            Then a(n) = T(r,c) where T(r,c) = 0 if r < c, T(r,c) =
 *            (r-c+1)(r-c+2)/2 if c ≤ r < 2c, T(r,c) = r² - 3(c...
 * Range:     u8 n <= 120, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t sum = 1;                 /* sum of first r integers */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* column c = n - (r-1)r/2 */
    seq_t prev = ((seq_t)r * (r - 1)) / 2;
    seq_t c = (seq_t)n - prev;

    seq_t nn = r;                  /* row as seq_t */
    seq_t kk = c;                  /* column as seq_t */

    /* triangle formula */
    if ((nn - kk) < kk) {          /* equivalent to r < 2k */
        seq_t a1 = nn - kk + 1;
        seq_t b1 = nn - kk + 2;
        return a1 * b1 / 2;
    } else {
        seq_t term1 = nn * nn;
        seq_t term2 = 3 * (kk - 1) * nn;
        seq_t term3_num = (5 * kk - 4) * (kk - 1);
        seq_t term3 = term3_num / 2;
        return term1 - term2 + term3;
    }
}
