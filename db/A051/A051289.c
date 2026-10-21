/*
 * A051289  Triangular array T read by rows: T(n,k)=P(2n+1,n,2k+1), where
 *          P(n,k,c)=number of vectors (x(1),x(2,),...,x(n)) of k 1's and n-k
 *          0's such that x(i)=x(n+1-i) for exactly c values of i.
 * https://oeis.org/A051289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r,k) * C(k,⌊k/2⌋) * 2^(r-k) * where r =
 *            floor((sqrt(8n+1)-1)/2) is the row number, * k = n - r(r+1)/2 is
 *            the column number within that row
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - i + 1) / i;   /* exact division at each step */
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r such that (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* position within the row */
    idx_t prev_total = r * (r + 1) / 2; /* elements before this row */
    idx_t k = n - prev_total;          /* column index in the row */

    /* compute the product */
    seq_t c1 = binomial(r, k);
    seq_t c2 = binomial(k, k / 2);     /* floor(k/2) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < r - k; ++i)
        pow2 *= 2;                     /* multiply by 2^(r-k) */

    return c1 * c2 * pow2;
}
