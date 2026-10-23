/*
 * A144912  Unreduced numerators of digital mean, dm_num(b, n), with rows n in
 *          {2, 3, 4, ...} and columns b in {2, 3, 4, ..., n}.
 * https://oeis.org/A144912
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, base, easy, frac, tabl
 * Formula:   a(n) = dm_num(b, r) where r and b are obtained from the flat
 *            index n by mapping to the triangle row r (n>=2) and column b
 *            (2<=b<=r). The unreduced numerator dm_num(b, r) equals the sum
 *            over the base‑b digits d_i of (2*d_i - (b-1))
 * Range:     i8 n <= 2211, i16 n <= 9289, i32 n <= 9289, i64 n <= 9289,
 *            i128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0. */
    if (n < 2) return (seq_t)0;

    /* Find the row r.  Let m = r-1 satisfy m*(m-1)/2 <= n-2 < (m+1)*m/2. */
    idx_t m = 0;
    while ((m + 1) * m / 2 <= n - 2) m++;

    idx_t r = m + 1;                     /* current row number */
    idx_t start = 2 + m * (m - 1) / 2;   /* first index in this row */
    idx_t pos = n - start;               /* 0‑based position within the row */
    idx_t b = 2 + pos;                   /* base for this element */

    /* Convert r to base b and accumulate digit sum and digit count. */
    seq_t digit_sum = 0;
    idx_t digits = 0;
    idx_t temp = r;
    while (temp > 0) {
        digit_sum += (seq_t)(temp % b);
        temp /= b;
        digits++;
    }

    /* Compute dm_num(b, r) = 2*digit_sum - digits*(b-1). */
    seq_t result = 2 * digit_sum - (seq_t)digits * (seq_t)(b - 1);
    return result;
}
