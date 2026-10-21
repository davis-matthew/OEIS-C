/*
 * A227053  a(n) = (n^3)! / (n^3-n)! = number of ways of placing n labeled
 *          balls into n^3 labeled boxes with at most one ball in each box.
 * https://oeis.org/A227053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3)!/(n^3-n)! (number of ways to place n labeled balls
 *            into n^3 labeled boxes with at most one ball per box)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 by definition */

    /* Compute m = n^3 - n.  All arithmetic is done in idx_t (uint64_t),
       which is sufficient for n up to at least 10,000 (n^3 = 10^12 < 2^64). */
    idx_t m = n * n * n - n;

    seq_t result = 1;               /* start with multiplicative identity */
    for (idx_t k = 1; k <= n; ++k) {
        /* Multiply by each integer in the range [m+1, m+n] = [n^3-n+1, n^3] */
        result *= (seq_t)(m + k);
    }
    return result;
}
