/*
 * A158858  Triangle T(n,k) =3^(k-1)*e(n,k) read by rows, where e(n,k)= (e(n -
 *          1, k)*e(n, k - 1) + 1)/e(n - 1, k - 1).
 * https://oeis.org/A158858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil( (sqrt(1+8n)-1)/2 ) and k = n -
 *            r*(r-1)/2. For k = 1: T(r,1) = 2r + 1 For k >= 2: T(r,k) =
 *            (r-k+1) * 3^e where e = k if k is odd k-2 if k is even The
 *            values are exact for all n for which the result fits in seq_t
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column (position within the row) */
    idx_t prev_sum = r * (r - 1) / 2;   /* sum of lengths of previous rows */
    idx_t k = n - prev_sum;             /* 1‑based column index */

    /* Base case: first column of each row */
    if (k == 1)
        return (seq_t)(2 * r + 1);

    /* Compute the multiplier (r-k+1) */
    seq_t mult = (seq_t)(r - k + 1);

    /* Determine the exponent for the power of 3 */
    idx_t exponent = (k & 1U) ? k : k - 2;

    /* Compute 3^exponent */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < exponent; ++i)
        pow3 *= (seq_t)3;

    return mult * pow3;
}
