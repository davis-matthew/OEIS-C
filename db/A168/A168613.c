/*
 * A168613  a(n) = 3^n - 5.
 * https://oeis.org/A168613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - 5
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    idx_t exp = n;

    while (exp != 0)
    {
        if (exp & 1U)        /* if lowest bit of exp is 1 */
            result *= base;  /* multiply result by current base */

        exp >>= 1;           /* shift exponent right */
        if (exp != 0)        /* avoid one unnecessary squaring */
            base *= base;    /* square the base for next bit */
    }

    result -= (seq_t)5;      /* subtract 5 to get 3^n - 5 */
    return result;
}
