/*
 * A084180  Duplicate of A020988.
 * https://oeis.org/A084180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 * (4^n - 1) / 3 (a(0) = 0)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponentiation by squaring to compute 4^n */
    seq_t result = 1;          /* will hold 4^n */
    seq_t base   = 4;          /* current power of 4 for squaring */
    idx_t e = n;               /* iterate over bits of the exponent */

    while (e)
    {
        if (e & 1)
            result *= base;   /* multiply when bit is set */
        base *= base;         /* square the base */
        e >>= 1;
    }

    /* compute (4^n - 1) * 2 / 3 */
    result -= 1;
    result *= 2;
    result /= 3;

    return result;
}
