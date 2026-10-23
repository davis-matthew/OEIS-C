/*
 * A143765  a(n+1) = a(n)^2 - 3*n*a(n) + n^2, a(1) = 1.
 * https://oeis.org/A143765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = a(n)^2 - 3*n*a(n) + n^2, a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) {
        /* the sequence is defined only for n >= 1 */
        return 0;
    }

    /* a(1) = 1 */
    seq_t cur = 1;

    /* iterate from 1 up to n-1 computing a(n) from the recurrence */
    for (idx_t i = 1; i < n; ++i) {
        idx_t k = i;                /* current n in the recurrence */
        seq_t next = cur * cur;     /* a(k)^2 */
        next = next - 3 * (seq_t)k * cur;  /* - 3*k*a(k) */
        next = next + (seq_t)k * (seq_t)k;  /* + k^2 */
        cur = next;
    }

    return cur;
}
