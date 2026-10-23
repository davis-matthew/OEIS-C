/*
 * A014994  a(n) = (1 - (-12)^n)/13.
 * https://oeis.org/A014994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-12)^n)/13 Recurrence: a(1)=1, a(2)=-11, a(n) =
 *            -11*a(n-1) + 12*a(n-2) for n>=3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; undefined for n<1 */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-11;

    seq_t prev2 = (seq_t)1;       /* a(1) */
    seq_t prev1 = (seq_t)-11;     /* a(2) */
    seq_t current;

    for (idx_t i = 3; i <= n; ++i) {
        current = (seq_t)-11 * prev1 + (seq_t)12 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
