/*
 * A140966  a(n) = (5 + (-2)^n)/3.
 * https://oeis.org/A140966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (5 + (-2)^n)/3 Recurrence: a(0) = 2, a(1) = 1, a(n) =
 *            -a(n-1) + 2*a(n-2) for n ≥ 2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)2; /* a(n-2) */
    seq_t a1 = (seq_t)1; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = -a1 + (seq_t)2 * a0; /* a(n) = -a(n-1) + 2*a(n-2) */
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
