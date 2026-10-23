/*
 * A100887  Expansion of (-1+2x+2x^2)/((1+x+x^2)(1-x-x^2)).
 * https://oeis.org/A100887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4); a(0) = -1, a(1) = 2, a(2) =
 *            1, a(3) = 0
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* We maintain the last four terms: a_{i-4}, a_{i-3}, a_{i-2}, a_{i-1}. */
    seq_t a0 = (seq_t)-1;      /* a_{i-4} */
    seq_t a1 = (seq_t)2;       /* a_{i-3} */
    seq_t a2 = (seq_t)1;       /* a_{i-2} */
    seq_t a3 = (seq_t)0;       /* a_{i-1} */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a2 + (seq_t)2 * a1 + a0;  /* a_i = a_{i-2} + 2*a_{i-3} + a_{i-4} */
        a0 = a1;   /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a3 now holds a(n) */
}
