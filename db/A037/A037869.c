/*
 * A037869  a(n) = (number of digits <=4)-(number of digits >4) in base 10
 *          representation of n.
 * https://oeis.org/A037869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (#digits <= 4) - (#digits > 4) in the base‑10
 *            representation of n
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t sum = 0;
    while (x > 0) {
        uint32_t d = x % 10;
        if (d <= 4)
            sum += 1;
        else
            sum -= 1;
        x /= 10;
    }
    return sum;
}
