/*
 * A143431  Periodic length 8 sequence [1, -1, 1, -1, -1, 1, -1, 1, ...].
 * https://oeis.org/A143431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = {1,-1,1,-1,-1,1,-1,1}[n mod 8]
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[8] = {
        (seq_t) 1, (seq_t)-1, (seq_t) 1, (seq_t)-1,
        (seq_t)-1, (seq_t) 1, (seq_t)-1, (seq_t) 1
    };
    return pat[n & 7];
}
