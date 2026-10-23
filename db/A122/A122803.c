/*
 * A122803  Powers of -2: a(n) = (-2)^n.
 * https://oeis.org/A122803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, changed
 * Formula:   a(n) = (-2)^n Recurrence: a(0) = 1, a(n) = -2 * a(n-1) for n > 0
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 1;      /* a(0) */
    seq_t step = -2;    /* factor for the recurrence */
    for (idx_t i = 0; i < n; ++i) {
        val *= step;    /* apply the recurrence */
    }
    return val;
}
