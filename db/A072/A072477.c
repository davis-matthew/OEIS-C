/*
 * A072477  a(n) = (2*n)!*binomial(2*n,n)/8.
 * https://oeis.org/A072477
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)! * binomial(2*n, n) / 8 Recurrence (valid for n >=
 *            3): a(2) = 18, a(n) = a(n-1) * 4 * (2*n - 1)^2 The
 *            implementation uses the recurrence which avoids factorials and
 *            works for any seq_t that supports the usual integer arithmetic
 *            operators. For n < 2 the function returns 0 (offset 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below offset are irrelevant   */
    seq_t res = 18;                /* a(2) = 18                            */
    /* At n = 2 we are done.  For n >= 3 apply the recurrence.      */
    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t x = (seq_t)(2 * k - 1);   /* 2k-1 fits in idx_t              */
        /* factor = 4 * (2k-1)^2                                      */
        seq_t factor = 4;
        factor *= x;
        factor *= x;
        res *= factor;                 /* res = a(k-1) * factor = a(k)     */
    }
    return res;
}
