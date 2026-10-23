/*
 * A187801  Pascal's triangle construction method applied to {1,1,2} as an
 *          initial term.
 * https://oeis.org/A187801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the triangle produced by starting with
 *            the row {1,1,2} and repeatedly applying the Pascal‑triangle
 *            convolution. The index n starts at 2, with a(2) = first entry of
 *            that triangle
 * Range:     u8 n <= 57, u16 n <= 195, u32 n <= 644, u64 n <= 1273,
 *            u128 n <= 1273, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient n choose k using a multiplicative
   algorithm.  The result is 0 if k > n. */
static seq_t binom_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;    /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n must be at least 2.  For n < 2 the definition is irrelevant. */
    if (n < 2) return 0;

    idx_t offset = n - 2;                /* 0‑based position in the triangle */
    idx_t r = 0;                         /* row index, starting from 0 */

    /* Find the row r such that the cumulative number of entries up
       to that row exceeds 'offset'.  The total number of entries
       in rows 0..r is (r+1)*(r+6)/2. */
    while ( ((r + 1) * (r + 6)) / 2 <= offset )
        ++r;

    /* Number of entries up to the previous row. */
    idx_t prev_total = (r * (r + 5)) / 2;
    idx_t j = offset - prev_total;      /* column index in row r */

    seq_t res = 0;
    /* a(n) = C(r,j) + C(r,j-1) + 2*C(r,j-2), treating any out‑of‑range
       binomial as 0. */
    if (j <= r) res += binom_mul(r, j);
    if (j >= 1 && j - 1 <= r) res += binom_mul(r, j - 1);
    if (j >= 2 && j - 2 <= r) res += (seq_t)2 * binom_mul(r, j - 2);

    return res;
}
