/*
 * A130778  Period 6: repeat [1, -1, -3, -3, -1, 1].
 * https://oeis.org/A130778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + a(n-3) for n>2, a(0) = 1, a(1) =
 *            -1, a(2) = -3
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;
    if (n == 2) return -3;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = -1;  /* a(n-2) */
    seq_t a2 = -3;  /* a(n-1) */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        a_next = 2 * a2 - 2 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
