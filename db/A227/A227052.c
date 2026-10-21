/*
 * A227052  a(n) = (n^2)! / (n^2-n)! = number of ways of placing n labeled
 *          balls into n^2 labeled boxes with at most one ball in each box.
 * https://oeis.org/A227052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2)! / (n^2 - n)! = ∏_{k=1}^{n} (n^2 - n + k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n == 0)
        return result;               /* a(0) = 1 */

    seq_t base  = (seq_t)n * n;      /* n^2  */
    seq_t start = base - n;          /* n^2 - n  */

    for (idx_t k = 1; k <= n; ++k) {
        result *= (start + (seq_t)k);
    }
    return result;
}
