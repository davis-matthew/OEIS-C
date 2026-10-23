/*
 * A157522  Triangle T(n, k) = f(n, k) + f(n, n-k) - 1, where f(n, k) = k if k
 *          <= floor(n/4), floor(n/2) - k if floor(n/4) < k <= floor(n/2), k -
 *          floor(n/2) if floor(n/2) < k <= floor(3*n/4), otherwise n-k, read
 *          by rows.
 * https://oeis.org/A157522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are the row and column indices of
 *            the triangle read by rows. T(r,k) = f(r,k) + f(r,r-k) - 1, with
 *            f defined for 0 ≤ k ≤ r by f(r,k) = k + 1 if k <= floor(r/4)
 *            f(r,k) = floor(r/2) - k + 1 if floor(r/4) < k <= floor(r/2)
 *            f(r,k) = k - floor(r/2) + 1 if floor(r/2) < k <= f...
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t f(idx_t r, idx_t k)
{
    idx_t q1 = r / 4;
    idx_t q2 = r / 2;
    idx_t q3 = (3 * r) / 4;

    if (k <= q1)                 return (seq_t)(k + 1);
    if (k <= q2)                 return (seq_t)(q2 - k + 1);
    if (k <= q3)                 return (seq_t)(k - q2 + 1);
    return (seq_t)(r - k + 1);
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t offset = 0;
    while (n >= offset + (r + 1)) {
        offset += r + 1;
        r++;
    }
    /* column index */
    idx_t k = n - offset;           /* 0 <= k <= r */

    seq_t f1 = f(r, k);
    seq_t f2 = f(r, r - k);
    return f1 + f2 - 1;
}
