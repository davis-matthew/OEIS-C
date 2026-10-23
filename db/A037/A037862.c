/*
 * A037862  a(n)=(number of digits <=1)-(number of digits >1) in base 3
 *          representation of n.
 * https://oeis.org/A037862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (#digits <=1) - (#digits >1) in base-3 representation of
 *            n
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    while (m) {
        idx_t d = m % 3;
        if (d <= 1)
            result++;
        else
            result--;
        m /= 3;
    }
    return result;
}
