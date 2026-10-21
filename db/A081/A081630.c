/*
 * A081630  a(n) = 2-(-3)^n.
 * https://oeis.org/A081630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2-(-3)^n
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;   /* current value of (-3)^k */
    seq_t base  = -3;  /* base of exponentiation */

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            power = power * base;
        base = base * base;
        exp >>= 1;
    }

    return 2 - power;
}
