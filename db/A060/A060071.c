/*
 * A060071  Duplicate of A037205.
 * https://oeis.org/A060071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^(n-1) - 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case n = 1 which gives 0. */
    if (n == 0)
        return -1;          /* undefined for offset 1, not expected */

    seq_t result = 1;        /* start with 1 for exponentiation */
    seq_t base   = (seq_t)n; /* base is n */
    idx_t exp    = n - 1;    /* exponent is n-1 */

    while (exp)
    {
        if (exp & 1)         /* if current bit is set, multiply */
            result *= base;
        base *= base;        /* square the base */
        exp >>= 1;           /* move to next bit */
    }

    /* subtract 1 to get a(n) */
    return result - 1;
}
