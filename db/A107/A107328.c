/*
 * A107328  Duplicate of A065034.
 * https://oeis.org/A107328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3) with a(0) = 3, a(1) = 4,
 *            a(2) = 8
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    const seq_t FOUR = (seq_t)4;
    seq_t a0 = (seq_t)3;   /* a(n-3)   */
    seq_t a1 = (seq_t)4;   /* a(n-2)   */
    seq_t a2 = (seq_t)8;   /* a(n-1)   */
    seq_t next;

    /* Compute terms iteratively up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        /* next = 4*a2 - 4*a1 + a0 */
        next = FOUR * a2 - FOUR * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
