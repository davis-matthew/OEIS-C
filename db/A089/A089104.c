/*
 * A089104  Duplicate of A007334.
 * https://oeis.org/A089104
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 * n^(n-3) (for n >= 3, a(2) = 1)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                 /* a(2) = 1, a(1) unused */
        return 1;

    seq_t base   = (seq_t)n;   /* start with n */
    idx_t exp   = n - 3;       /* exponent n-3 */
    seq_t result = 1;          /* accumulator for n^(n-3) */

    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)2 * result;  /* final 2*n^(n-3) */
}
