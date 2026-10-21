/*
 * A231345  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the odd numbers interleaved with k-1 zeros but T(n,1) = -1 and the
 *          first element of column k is in row k(k+1)/2.
 * https://oeis.org/A231345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, nice
 * Formula:   not stated
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A231345: triangle read by rows with odd values interleaved with zeros. */
seq_t a(idx_t n)
{
    /* find the row r containing the n-th term */
    idx_t r = 1;
    idx_t cumulative = 0;
    while (1)
    {
        /* compute length of row r: largest k with k(k+1)/2 <= r */
        idx_t kmax = 0;
        while ((kmax + 1) * (kmax + 2) / 2 <= r)
            ++kmax;
        if (cumulative + kmax >= n)
            break;
        cumulative += kmax;
        ++r;
    }

    /* position of the term within the row (0‑based) */
    idx_t pos_in_row = n - cumulative - 1;
    idx_t k = pos_in_row + 1;    /* column index (1‑based) */

    if (k == 1)
        return (seq_t)-1;

    /* first row of column k is r0 = k(k+1)/2 */
    idx_t r0 = k * (k + 1) / 2;
    /* r is guaranteed to satisfy r >= r0 because k <= kmax */
    idx_t diff = r - r0;
    if (diff % k == 0)   /* odd number */
    {
        idx_t m = diff / k + 1;          /* odd index (1‑based) */
        seq_t val = (seq_t)2 * (seq_t)m - 1;
        return val;
    }
    else                  /* zero entry */
    {
        return (seq_t)0;
    }
}
