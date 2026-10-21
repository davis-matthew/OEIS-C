/*
 * A093112  a(n) = (2^n-1)^2 - 2.
 * https://oeis.org/A093112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2^n-1)^2 - 2 (Offset 1)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Powers of two via repeated multiplication; safe for arbitrary-precision types */
    seq_t pow = 1;
    seq_t two = 2;

    for (idx_t i = 0; i < n; ++i) {
        pow *= two;
    }

    seq_t val = pow - 1;   /* (2^n - 1) */
    seq_t res = val * val; /* square */
    res -= 2;              /* subtract 2 */

    return res;
}
