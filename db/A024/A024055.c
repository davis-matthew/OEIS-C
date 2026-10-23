/*
 * A024055  a(n) = 5^n - n^6.
 * https://oeis.org/A024055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^6
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 5^n by binary exponentiation */
    seq_t pow5 = (seq_t)1;
    seq_t base = (seq_t)5;
    idx_t e = n;
    while (e != 0) {
        if (e & 1) pow5 = pow5 * base;
        base = base * base;
        e >>= 1;
    }

    /* compute n^6 by multiplying n six times */
    seq_t n_seq = (seq_t)n;
    seq_t n6    = (seq_t)1;
    for (int i = 0; i < 6; ++i) n6 = n6 * n_seq;

    return pow5 - n6;
}
