/*
 * A073466  Triangle T(j,k) = remainder when j-th triangular number is divided
 *          by k-th triangular number, for 2 < j and 1 < k < j.
 * https://oeis.org/A073466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(j) mod T(k), where T(x) = x*(x+1)/2. For n ≥ 0, j is
 *            the smallest integer ≥ 3 such that the cumulative count of terms
 *            up to row j exceeds n: (j-3)(j-2)/2 ≤ n < (j-2)(j-1)/2. Then let
 *            m = j-2. The offset within that row is offset = n - (m-1)*m/2,
 *            and k = offset + 2. This gives the remaind...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t triangular(idx_t x)
{
    return (seq_t)x * ((seq_t)x + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find m = j-2 such that m*(m+1)/2 > n. */
    idx_t m = 0;
    while (((seq_t)m * (m + 1)) / 2 <= n)
        ++m;

    idx_t j = m + 2;

    /* Number of terms before this row. */
    idx_t prev = (idx_t)(((seq_t)(m - 1) * (seq_t)m) / 2);
    idx_t offset = n - prev;        /* 0 ≤ offset < m */

    idx_t k = offset + 2;

    seq_t tj = triangular(j);
    seq_t tk = triangular(k);

    return tj % tk;
}
