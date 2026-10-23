/*
 * A035174  Ramanujan's tau function (or tau numbers (A000594)) for 2^n.
 * https://oeis.org/A035174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+2) = -24*a(n+1) - 2048*a(n), a(0)=1, a(1)=-24
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-24;

    seq_t prevprev = (seq_t)1;
    seq_t prev     = (seq_t)-24;
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)-24 * prev - (seq_t)2048 * prevprev;
        prevprev = prev;
        prev = current;
    }

    return prev;
}
