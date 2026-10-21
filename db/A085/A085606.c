/*
 * A085606  a(n) = (n-1)^n - 1.
 * https://oeis.org/A085606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)^n - 1, offset 0
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* a(0) = 0 */
        return 0;

    seq_t base = (seq_t)(n - 1);   /* (n-1) */
    seq_t result = 1;              /* accumulator for power */
    idx_t exp = n;                 /* exponent n */

    /* Binary exponentiation: compute (n-1)^n */
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;        /* multiply when the bit is 1 */
        base *= base;             /* square the base */
        exp >>= 1;                /* shift exponent right */
    }

    return result - 1;             /* subtract 1 */
}
