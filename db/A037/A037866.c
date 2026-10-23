/*
 * A037866  a(n)=(number of digits <=3)-(number of digits >3) in base 7
 *          representation of n.
 * https://oeis.org/A037866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (number of digits ≤ 3) – (number of digits > 3) in base 7
 *            representation of n
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t diff = 0;
    idx_t m = n;
    while (m) {
        unsigned digit = (unsigned)(m % 7);
        if (digit <= 3)
            diff++;
        else
            diff--;
        m /= 7;
    }
    return diff;
}
