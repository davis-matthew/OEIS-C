/*
 * A096661  Fine's numbers J(n).
 * https://oeis.org/A096661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{m>=1, n≥1} (-1)^{m + k} where * s = (3*m^2 + m)/2,
 *            diff = n - s, * if diff is non‑negative and divisible by m then
 *            k = diff / m; * otherwise the term contributes 0. This is a
 *            direct expansion * of the generating function Sum_{m>=1} (-1)^m
 *            q^{(3m^2+m)/2}/(1+q^m)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first two terms are zero for all n >= 0. */
    if (n < 2)
        return 0;

    seq_t sum = 0;

    /* Iterate over the “m” index of the series.  The inequality
     * (3*m^2+m)/2 <= n guarantees that the sum truncates.  Up to n=10000
     * this loop runs only about 115 times. */
    for (idx_t m = 1; ; ++m)
    {
        /* Compute s = (3*m^2 + m) / 2 safely using larger integer width. */
        __uint128_t mm = m;
        __uint128_t s128 = (3 * mm * mm + mm) >> 1;
        if (s128 > n)                     /* n is idx_t, small enough to compare */
            break;
        idx_t s = (idx_t)s128;            /* safe cast because s <= n */

        idx_t diff = n - s;
        if ((diff % m) == 0)
        {
            idx_t k = diff / m;
            /* add or subtract 1 depending on parity of (m + k) */
            if (((m + k) & 1) == 0)
                sum += 1;
            else
                sum -= 1;
        }
    }

    return sum;
}
