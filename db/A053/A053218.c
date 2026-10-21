/*
 * A053218  Triangle read by rows: T(n,k) = T(n,k-1) + T(n-1,k-1) for k >= 2
 *          with T(n,1) = n.
 * https://oeis.org/A053218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n – r(r−1)/2, and T(r,k) = r·2^(k−1) – (k−1)·2^(k−2) (T(r,1)=r)
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – n≥1 normally */

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column index k within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* direct value for the first column */
    if (k == 1)
        return (seq_t)r;

    /* compute 2^(k-2) and 2^(k-1) */
    seq_t pow2 = (seq_t)1;                 /* 2^(k-2) */
    for (idx_t i = 1; i < k - 1; ++i)
        pow2 *= (seq_t)2;

    seq_t pow1 = pow2 * (seq_t)2;           /* 2^(k-1) */

    /* T(r,k) = r*pow1 – (k-1)*pow2 */
    seq_t term = (seq_t)r * pow1 - (seq_t)(k - 1) * pow2;
    return term;
}
