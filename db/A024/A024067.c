/*
 * A024067  a(n) = 6^n - n^5.
 * https://oeis.org/A024067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^5
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 6^n with binary exponentiation */
    seq_t pow6  = 1;
    seq_t base  = (seq_t)6;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            pow6 = pow6 * base;
        base = base * base;
        exp >>= 1;
    }

    /* Compute n^5 directly */
    seq_t nn   = (seq_t)n;
    seq_t pow5 = nn * nn * nn * nn * nn;

    return pow6 - pow5;
}
