/*
 * A132465  Let df(n,k) = Product_{i=0..k-1} (n-i) be the descending factorial
 *          and let P(m,n) = df(n-1,m-1)^2*(2*n-m)/((m-1)!*m!). Sequence gives
 *          P(7,n).
 * https://oeis.org/A132465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6))^2 * (2*n-7) /
 *            3628800
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All values fit exactly into seq_t when it is large enough;
       for 64‑bit builds the sequence is only checked for values
       that still fit in 64 bits. */
    if (n < 7) {
        return (seq_t)0;
    }

    /* Compute the falling product (n‑1)(n‑2)…(n‑6) */
    seq_t prod = (seq_t)1;
    /* loop counter uses idx_t to avoid unsigned wrap‑around problems */
    for (idx_t i = 0; i < 6; ++i) {
        prod *= (seq_t)(n - 1 - i);
    }

    /* Square the product */
    seq_t sq = prod * prod;

    /* Compute (2*n - 7) as seq_t and multiply */
    seq_t twon = (seq_t)n * 2;
    twon -= (seq_t)7;
    seq_t numerator = sq * twon;

    /* Divider 10! = 3628800 */
    const seq_t denom = (seq_t)3628800ULL;

    return numerator / denom;
}
