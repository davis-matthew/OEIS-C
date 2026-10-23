/*
 * A024048  a(n) = 4^n - n^12.
 * https://oeis.org/A024048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^n by binary exponentiation */
    seq_t pow4 = (seq_t)1;
    seq_t base = (seq_t)4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U)
            pow4 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Compute n^12 by repeated multiplication */
    seq_t n12 = (seq_t)n;
    for (idx_t i = 1; i < 12; ++i)
        n12 *= (seq_t)n;

    return pow4 - n12;
}
