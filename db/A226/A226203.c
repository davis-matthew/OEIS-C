/*
 * A226203  a(5n) = a(5n+3) = a(5n+4) = 2n+1, a(5n+1) = 2n-3, a(5n+2) = 2n-1.
 * https://oeis.org/A226203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(5n) = a(5n+3) = a(5n+4) = 2n+1, a(5n+1) = 2n-3, a(5n+2) = 2n-1
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Get quotient and remainder of n divided by 5 using unsigned arithmetic. */
    idx_t q = n / 5;           /* floor(n / 5)  */
    idx_t r = n % 5;           /* n mod 5       */

    /* Convert the quotient to seq_t for signed arithmetic. */
    seq_t sq = (seq_t)q;

    /* Apply the piecewise definition. */
    switch (r) {
        case 0:
        case 3:
        case 4:
            return sq * 2 + 1;
        case 1:
            return sq * 2 - 3;
        case 2:
            return sq * 2 - 1;
    }
    /* Should never reach here; return 0 as a safe default. */
    return (seq_t)0;
}
