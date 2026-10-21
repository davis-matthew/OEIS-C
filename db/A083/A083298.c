/*
 * A083298  Duplicate of A083222.
 * https://oeis.org/A083298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 5^n if n is even a(n) = 3 * 5^(n-1) if n is odd
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t i;

    if ((n & 1U) == 0)           /* n is even: a(n) = 5^n */
    {
        for (i = 0; i < n; ++i)
            result *= (seq_t)5;
    }
    else                          /* n is odd: a(n) = 3*5^(n-1) */
    {
        for (i = 0; i < n-1; ++i)
            result *= (seq_t)5;
        result *= (seq_t)3;
    }

    return result;
}
