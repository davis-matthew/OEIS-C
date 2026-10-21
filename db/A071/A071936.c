/*
 * A071936  K(n+1,n) where K(x,y) is the Kronecker symbol (x/y).
 * https://oeis.org/A071936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -1 if n mod 8 == 2; otherwise a(n) = +1
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 (offset 1) */
    if ((n & 7ULL) == 2ULL) {
        return (seq_t)-1;
    }
    return (seq_t)1;
}
