/*
 * A047907  a(n+1) = a(n) - n (if n is even), a(n+1) = a(n) * n (if n is odd).
 * https://oeis.org/A047907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n+1) = a(n) - n if n is even, a(n+1) = a(n) * n if n is odd
 *            (a(1) = 1)
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 21, i64 n <= 33, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = 1;
    if (n <= 1) return val;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t prev = i - 1;
        seq_t p = (seq_t)prev;
        if ((prev & 1u) == 0) {        /* prev even */
            val = val - p;
        } else {                      /* prev odd  */
            val = val * p;
        }
    }
    return val;
}
