/*
 * A095887  Triangle read by rows: T(n,k) = (n-k+1)^n, n>=1, 1<=k<=n.
 * https://oeis.org/A095887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((row - k) ^ row) where row is the smallest integer with
 *            row*(row+1)/2 ≥ n, k = n - row*(row-1)/2 - 1 (0-based index in
 *            the row)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 120, u128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row */
    idx_t row = 1;
    idx_t cum = 1;               /* cumulative terms up to current row */
    while (cum < n) {
        row++;
        cum += row;
    }

    /* previous cumulative terms (before this row) */
    idx_t prev = cum - row;
    /* 0‑based offset in the row */
    idx_t offset = n - prev - 1;
    /* base to raise: (row - offset) = (row - (k-1)) = (row - k + 1) = (n - offset)??? */
    idx_t base_int = row - offset;   /* integer base, in [1, row] */

    /* convert to seq_t and compute power by binary exponentiation */
    seq_t result = 1;
    seq_t base   = (seq_t)base_int;
    idx_t exp    = row;
    while (exp > 0) {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;   /* avoid one extra multiplication when exp becomes 0 */
    }

    return result;
}
