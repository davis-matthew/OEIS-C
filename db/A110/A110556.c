/*
 * A110556  a(n) = binomial(2*n-1, n)*(-1)^n.
 * https://oeis.org/A110556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(2*n-1, n) * (-1)^n
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Use 128-bit intermediate to avoid overflow in 64‑bit builds */
        __int128 tmp = (__int128)result * (n - 1 + i);
        tmp /= i;            /* exact division */
        result = (seq_t)tmp;
    }
    if (n & 1)   /* odd n ⇒ negative sign */
        return -result;
    else
        return result;
}
