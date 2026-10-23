/*
 * A135098  Duplicate of A136488.
 * https://oeis.org/A135098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3); a(0)=1, a(1)=2, a(2)=5
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    seq_t a0 = 1, a1 = 2, a2 = 5;
    seq_t ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 * 2 + a1 * 2 - a0 * 4;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
