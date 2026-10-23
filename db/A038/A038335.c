/*
 * A038335  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*9^j.
 * https://oeis.org/A038335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,c) * 12^(r-c) * 9^c where r = largest integer
 *            such that r*(r+1)/2 <= n, and c = n - r*(r+1)/2 (the column in
 *            row r)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 37, u64 n <= 138, u128 n <= 474,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and the index c within that row. */
    idx_t r = 0;
    idx_t sum = 0;          /* total terms up to (but not including) row r */
    while (sum + r + 1 <= n) {
        sum += r + 1;
        r++;
    }
    idx_t c = n - sum;     /* 0 <= c <= r */

    /* Compute binomial coefficient binomial(r,c). */
    seq_t bin = 1;
    idx_t k = c;
    if (k > r - k)          /* capitalize symmetry */
        k = r - k;
    for (idx_t i = 1; i <= k; ++i) {
        bin = bin * (r - k + i) / i;
    }

    /* Multiply by 12^(r-c) and 9^c. */
    seq_t val = bin;
    for (idx_t i = 0; i < r - c; ++i)
        val *= 12;
    for (idx_t i = 0; i < c; ++i)
        val *= 9;

    return val;
}
