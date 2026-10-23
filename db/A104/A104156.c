/*
 * A104156  a(1)=a(2)=0, a(n) = abs(2*a(n-1) - a(n-2)) - 1.
 * https://oeis.org/A104156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = abs(2*a(n-1) - a(n-2)) - 1, a(1) = 0, a(2) = 0
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2)
        return 0;

    seq_t a_prev2 = 0; /* a(1) */
    seq_t a_prev1 = 0; /* a(2) */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = (seq_t)2 * a_prev1 - a_prev2; /* 2*a(n-1) - a(n-2) */
        if (temp < 0)
            temp = -temp;              /* abs */
        a_cur = temp - 1;               /* -1 */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
