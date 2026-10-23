/*
 * A108033  n!*(3*n^2-13*n+14)/6.
 * https://oeis.org/A108033
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (3*n^2 - 13*n + 14) / 6 (offset 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)                   /* values undefined below the offset */
        return 0;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute the polynomial part */
    seq_t t = (seq_t)3 * (seq_t)n * (seq_t)n
              - (seq_t)13 * (seq_t)n
              + (seq_t)14;

    /* divide the factorial first to keep the intermediate small */
    fact /= (seq_t)6;

    return fact * t;
}
