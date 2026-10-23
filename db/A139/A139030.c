/*
 * A139030  Real part of (4 + 3i)^n.
 * https://oeis.org/A139030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2), a(0)=1, a(1)=4
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)4;
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)8 * prev1 - (seq_t)25 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
