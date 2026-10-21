/*
 * A098251  Chebyshev polynomials S(n,363).
 * https://oeis.org/A098251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 363*a(n-1) - a(n-2); a(0) = 1; a(1) = 363
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)363;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)363; /* a(1) */
    seq_t cur = prev1;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = (seq_t)363 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
