/*
 * A022109  Fibonacci sequence beginning 1, 19.
 * https://oeis.org/A022109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 19
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t f_prev = (seq_t)1;   /* a(0) */
    seq_t f_cur  = (seq_t)19;  /* a(1) */
    seq_t f_next;

    /* iterate from 2 up to n, inclusive */
    for (idx_t i = 2; i <= n; ++i) {
        f_next = f_prev + f_cur; /* recurrence */
        f_prev = f_cur;
        f_cur  = f_next;
    }
    return f_cur;
}
