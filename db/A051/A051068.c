/*
 * A051068  Duplicate of A050294.
 * https://oeis.org/A051068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Σ_{k≥0} (-1)^k · ⌊ n / 3^k ⌋
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  sum = 0;
    seq_t  pow = 1;      /* current 3^k */
    int    sign = 1;     /* (-1)^k */

    while (pow <= n) {
        seq_t term = (seq_t)(n / pow);  /* ⌊n/3^k⌋ as seq_t */

        if (sign > 0)
            sum += term;
        else
            sum -= term;

        sign = -sign;                     /* toggle sign for next k */

        /* avoid overflow of pow when we multiply by 3 */
        if (pow > n / 3) break;
        pow *= 3;
    }
    return sum;
}
