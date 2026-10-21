/*
 * A061084  Fibonacci-type sequence based on subtraction: a(0) = 1, a(1) = 2
 *          and a(n) = a(n-2) - a(n-1).
 * https://oeis.org/A061084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(0) = 1, a(1) = 2, a(n) = a(n-2) – a(n-1)
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 45, i64 n <= 91, i128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_prev2 = (seq_t)1;   /* a(n-2)   for current n */
    seq_t a_prev1 = (seq_t)2;   /* a(n-1)   for current n */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr   = a_prev2 - a_prev1; /* a(i) = a(i-2) - a(i-1) */
        a_prev2  = a_prev1;           /* shift window forward   */
        a_prev1  = a_curr;
    }
    return a_prev1; /* a(n) */
}
