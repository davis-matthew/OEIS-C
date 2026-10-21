/*
 * A083085  a(n) = (2+(-5)^n)/3.
 * https://oeis.org/A083085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2 + (-5)^n) / 3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t one = 1;
    seq_t two = 2;
    seq_t three = 3;
    seq_t five = 5;
    seq_t negFive = -five;

    seq_t pow_val = one;      /* initialise (-5)^0 = 1   */
    seq_t base   = negFive;   /* base for exponentiation */
    idx_t  e = n;

    /* exponentiation by squaring: pow_val = (-5)^n */
    while (e > 0) {
        if (e & 1) pow_val = pow_val * base;
        base = base * base;
        e >>= 1;
    }

    /* compute (2 + pow_val) / 3 */
    return (two + pow_val) / three;
}
