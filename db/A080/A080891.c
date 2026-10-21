/*
 * A080891  Period 5: repeat [0, 1, -1, -1, 1].
 * https://oeis.org/A080891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = 0 if n%5==0; 1 if n%5==1 or 4; -1 otherwise
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 5;
    if (r == 0) {
        return (seq_t)0;
    } else if (r == 1 || r == 4) {
        return (seq_t)1;
    } else {
        return (seq_t)-1;
    }
}
